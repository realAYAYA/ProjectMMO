// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/GA_Melee.h"

#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"

#include "Characters/MCharacter.h"
#include "GameplayAbilitySystem/AttributeSets/MAttributeSet.h"
#include "GameplayAbilitySystem/GameplayEffects/MGameplayEffect.h"


UGA_Melee::UGA_Melee()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

EActivateFailCode UGA_Melee::CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const
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
		FailCode =  EActivateFailCode::OutOfRange;
		Caster->OnAbilityFailed.Broadcast(FailCode);
		return FailCode;
	}

	// 面向敌人
	const FVector Dir = UKismetMathLibrary::Normal(Target->GetActorLocation() - Caster->GetActorLocation(), 0.0001);
	if (UKismetMathLibrary::Dot_VectorVector(Dir, Caster->GetActorForwardVector()) < 0.75f)
	{
		FailCode = EActivateFailCode::NoToward;
		Caster->OnAbilityFailed.Broadcast(FailCode);
		return FailCode;
	}

	// 计算消耗
	if (Caster->GetAttributeSet()->Rage.GetCurrentValue() < Rage)
	{
		FailCode = EActivateFailCode::NoRage;
		Caster->OnAbilityFailed.Broadcast(FailCode);
		return FailCode;
	}
	
	if (Caster->GetAttributeSet()->Energy.GetCurrentValue() < Energy)
	{
		FailCode = EActivateFailCode::NoEnergy;
		Caster->OnAbilityFailed.Broadcast(FailCode);
		return FailCode;
	}
	
	Caster->OnAbilityFailed.Broadcast(FailCode);

	return FailCode;
}

void UGA_Melee::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);

	// 根据目标类型选择目标
	const AMCharacter* Caster = Cast<AMCharacter>(OwnerInfo->AvatarActor.Get());
	const AMCharacter* Target = Caster->CurrentTarget;
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
}

