// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/GA_CastSpell.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"

#include "Characters/MCharacter.h"
#include "GameplayAbilitySystem/AttributeSets/MAttributeSet.h"
#include "GameplayAbilitySystem/GameplayEffects/MGameplayEffect.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tasks/AbilityTask_CastSpell.h"

UGA_CastSpell::UGA_CastSpell()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	TargetType = ETargetType::Hostile;

	Range = 1500;
}

EActivateFailCode UGA_CastSpell::CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const
{
	const AMCharacter* Caster = Cast<AMCharacter>(ActorInfo.AvatarActor.Get());
	
	EActivateFailCode FailCode = Super::CanActivateCondition(ActorInfo);
	if (FailCode != EActivateFailCode::Success)
	{
		Caster->OnAbilityFailed.Broadcast(FailCode);
		return FailCode;
	}
	
	const AMCharacter* Target = Caster->CurrentTarget;
	if (!Target || !Target->GetAbilitySystemComponent())
	{
		Caster->OnAbilityFailed.Broadcast(EActivateFailCode::NoTarget);
		return EActivateFailCode::NoTarget;
	}

	FailCode = EActivateFailCode::Success;
	
	// 目标类型不对
	
	// 太远了
	if ((Target->GetActorLocation() - Caster->GetActorLocation()).Length() > Range)
	{
		FailCode = EActivateFailCode::OutOfRange;
		Caster->OnAbilityFailed.Broadcast(FailCode);
		return FailCode;
	}

	// 消耗条件不足
	if (Caster->GetAttributeSet()->Mana.GetCurrentValue() < Mana)
	{
		FailCode = EActivateFailCode::NoMana;
		Caster->OnAbilityFailed.Broadcast(FailCode);
		return FailCode;
	}
	
	// 如果是目标敌对，需要面向对方，友方增益buff则不需要
	if (TargetType == ETargetType::Hostile)
	{
		const FVector Dir = UKismetMathLibrary::Normal(Target->GetActorLocation() - Caster->GetActorLocation(), 0.0001);
		if (UKismetMathLibrary::Dot_VectorVector(Dir, Caster->GetActorForwardVector()) < 0.7f)
		{
			FailCode = EActivateFailCode::NoToward;
			Caster->OnAbilityFailed.Broadcast(FailCode);
			return FailCode;
		}
	}

	Caster->OnAbilityFailed.Broadcast(FailCode);
	
	return FailCode;
}

void UGA_CastSpell::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
	
	AMCharacter* Caster = GetMCharacterFromActorInfo();
	SpellTask = UAbilityTask_CastSpell::CreateCastSpellTask(this, Caster, CastTime);
	SpellTask->OnAbilityTaskEnd.AddDynamic(this, &UGA_CastSpell::K2_EndAbility);
	SpellTask->OnAbilityCancel.AddDynamic(this, &UGA_CastSpell::K2_CancelAbility);
	SpellTask->ReadyForActivation();// 启动任务
}

void UGA_CastSpell::CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	if (SpellTask)
		SpellTask->EndTask();
}

void UGA_CastSpell::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	if (CanActivateCondition(*ActorInfo) != EActivateFailCode::Success)
	{
		bWasCancelled = true;
	}

	if (SpellTask)
		SpellTask->EndTask();

	// 根据目标类型选择目标
	const AMCharacter* Caster = GetMCharacterFromActorInfo();
	const AMCharacter* Target = nullptr;
	switch (TargetType)
	{
	default:break;
	case ETargetType::Self: Target = Caster;
	case ETargetType::Friendly:
		// Todo 如果目标是敌对的则选择自己
		Target = Caster->CurrentTarget;
	case ETargetType::Hostile:
		Target = Caster->CurrentTarget;
	}

	UAbilitySystemComponent* TargetComponent = Target->GetAbilitySystemComponent();

	// 对目标施加效果
	for (const TSubclassOf<UMGameplayEffect>& Effect : EffectsToTarget)
	{
		if (!Effect.Get())
			continue;
		
		const FGameplayEffectContextHandle EffectContext = TargetComponent->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = TargetComponent->MakeOutgoingSpec(Effect, Level, EffectContext);
		if (SpecHandle.IsValid())
		{
			const FActiveGameplayEffectHandle ActiveGEHandle = TargetComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			if (!ActiveGEHandle.WasSuccessfullyApplied())
				ABILITY_LOG(Log, TEXT("Ability %s faild to apply Effect to Target %s"), *GetName(), *GetNameSafe(Effect));
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
