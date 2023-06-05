// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/GA_Melee.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"

#include "Characters/MCharacter.h"
#include "GameplayAbilitySystem/GameplayEffects/MGameplayEffect.h"

UGA_Melee::UGA_Melee()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

EActivateFailCode UGA_Melee::CanActivateCondition() const
{
	if (Super::CanActivateCondition() == EActivateFailCode::Error)
		return EActivateFailCode::Error;

	const AMCharacter* Caster = GetMCharacterFromActorInfo();
	const AMCharacter* Target = Caster->GetCurrentTarget();
	if (!Target || !Target->GetAbilitySystemComponent())
		return EActivateFailCode::Error;

	// 目标类型不对
	
	// 太远了
	if ((Target->GetCurrentTarget()->GetActorLocation() - Caster->GetActorLocation()).Length() > Range)
		return EActivateFailCode::OutOfRange;
	
	return EActivateFailCode::Success;
}

void UGA_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);

	// 根据目标类型选择目标
	const AMCharacter* Caster = GetMCharacterFromActorInfo();
	const AMCharacter* Target = Caster->GetCurrentTarget();

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
}

