// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/GA_CastSpell.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"

#include "Characters/MCharacter.h"
#include "GameplayAbilitySystem/GameplayEffects/MGameplayEffect.h"

void UGA_CastSpell::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}

void UGA_CastSpell::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
}

bool UGA_CastSpell::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_CastSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);

	// 根据目标类型选择目标
	const AMCharacter* Caster = GetMCharacterFromActorInfo();
	const AMCharacter* Target = nullptr;
	switch (TargetType)
	{
	default:break;
	case ETargetType::Self: Target = Caster;
	case ETargetType::Friendly:
		// Todo 如果目标是敌对的则选择自己
	case ETargetType::Hostile:
		Target = Caster->GetCurrentTarget();
	}

	// 对目标施加效果
	if (EffectToTarget.Get() && Target)
	{
		if (UAbilitySystemComponent* TargetComponent = Target->GetAbilitySystemComponent())
		{
			const FGameplayEffectContextHandle EffectContext = OwnerInfo->AbilitySystemComponent->MakeEffectContext();
			const FGameplayEffectSpecHandle SpecHandle = TargetComponent->MakeOutgoingSpec(EffectToTarget, Level, EffectContext);
			if (!SpecHandle.IsValid())
			{
				const FActiveGameplayEffectHandle ActiveGEHandle = TargetComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				if (!ActiveGEHandle.WasSuccessfullyApplied())
					ABILITY_LOG(Log, TEXT("Ability %s faild to apply Effect to Target %s"), *GetName(), *GetNameSafe(EffectToTarget));
			}
		}
	}
}

void UGA_CastSpell::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_CastSpell::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!CanActivateCondition(ActorInfo))
	{
		bWasCancelled = true;
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
