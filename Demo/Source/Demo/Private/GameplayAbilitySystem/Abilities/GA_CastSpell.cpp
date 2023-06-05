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
}

EActivateFailCode UGA_CastSpell::CanActivateCondition() const
{
	if (Super::CanActivateCondition() == EActivateFailCode::Error)
		return EActivateFailCode::Error;

	const AMCharacter* Caster = GetMCharacterFromActorInfo();
	const AMCharacter* Target = Caster->GetCurrentTarget();
	if (!Target || !Target->GetAbilitySystemComponent())
		return EActivateFailCode::Error;

	// 目标类型不对
	
	// 太远了
	if ((Target->GetActorLocation() - Caster->GetActorLocation()).Length() > Range)
		return EActivateFailCode::OutOfRange;
	
	// 如果是目标敌对，需要面向对方，友方增益buff则不需要
	if (TargetType == ETargetType::Hostile)
	{
		const FVector Dir = UKismetMathLibrary::Normal(Target->GetActorLocation() - Caster->GetActorLocation(), 0.0001);
		if (UKismetMathLibrary::Dot_VectorVector(Dir, Caster->GetActorForwardVector()) < 0.5f)
			return EActivateFailCode::NoToward;
	}
	
	// 消耗条件不足
	if (Caster->GetAttributeSet()->Mana.GetCurrentValue() < Mana)
	{
		return EActivateFailCode::NoMana;
	}

	if (Caster->GetAttributeSet()->Rage.GetCurrentValue() < Rage)
	{
		return EActivateFailCode::NoRage;
	}
	
	if (Caster->GetAttributeSet()->Energy.GetCurrentValue() < Energy)
	{
		return EActivateFailCode::NoEnergy;
	}
	
	return EActivateFailCode::Success;
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

void UGA_CastSpell::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	if (SpellTask)
		SpellTask->EndTask();
}

void UGA_CastSpell::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (CanActivateCondition() != EActivateFailCode::Success)
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
		Target = Caster->GetCurrentTarget();
	case ETargetType::Hostile:
		Target = Caster->GetCurrentTarget();
	}

	// 对目标施加效果
	for (const TSubclassOf<UMGameplayEffect>& Effect : EffectsToTarget)
	{
		if (Effect.Get() && Target)
		{
			if (UAbilitySystemComponent* TargetComponent = Target->GetAbilitySystemComponent())
			{
				const FGameplayEffectContextHandle EffectContext = TargetComponent->MakeEffectContext();
				const FGameplayEffectSpecHandle SpecHandle = TargetComponent->MakeOutgoingSpec(Effect, Level, EffectContext);
				if (SpecHandle.IsValid())
				{
					const FActiveGameplayEffectHandle ActiveGEHandle = TargetComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					if (!ActiveGEHandle.WasSuccessfullyApplied())
						ABILITY_LOG(Log, TEXT("Ability %s faild to apply Effect to Target %s"), *GetName(), *GetNameSafe(Effect));
				}
			}
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
