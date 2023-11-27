// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/Base/GA_Base.h"
#include "GameplayAbilitySystem/Components/MAbilitySystemComponent.h"
#include "Characters/MCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemLog.h"

AMCharacter* UGA_Base::GetMCharacterFromActorInfo() const
{
	return Cast<AMCharacter>(GetAvatarActorFromActorInfo());
}

UMAbilitySystemComponent* UGA_Base::GetMASC() const
{
	return Cast<UMAbilitySystemComponent>(GetAvatarActorFromActorInfo());
}


EActivateFailCode UGA_Base::CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const
{
	return EActivateFailCode::Success;
}

void UGA_Base::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = OwnerInfo->AbilitySystemComponent.Get();

	const FGameplayEffectContextHandle EffectContext = OwnerInfo->AbilitySystemComponent->MakeEffectContext();

	// 处理技能开始时施加效果
	for (TSubclassOf<UGameplayEffect>& GameplayEffect : OngoingEffectsToJustApplyOnStart)
	{
		if (!GameplayEffect.Get())
			continue;
		
		const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, Level, EffectContext);
		if (!SpecHandle.IsValid())
			continue;

		FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		if (!ActiveGEHandle.WasSuccessfullyApplied())
			ABILITY_LOG(Log, TEXT("Ability %s faild to apply Startup Effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
	}

	if (!this->IsInstantiated())
		return;

	// 处理技能结束时施加效果
	for (TSubclassOf<UGameplayEffect>& GameplayEffect : OngoingEffectsToRemoveOnEnd)
	{
		if (!GameplayEffect.Get())
			continue;

		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, Level, EffectContext);
		if (!SpecHandle.IsValid())
			continue;
		
		FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		if (ActiveGEHandle.WasSuccessfullyApplied())
			RemoveOnEndEffectHandles.Add(ActiveGEHandle);
		else
			ABILITY_LOG(Log, TEXT("Ability %s faild to apply startup effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
	}
}

void UGA_Base::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (!this->IsInstantiated())
		return;
	
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	for (FActiveGameplayEffectHandle& ActiveGEHandle : RemoveOnEndEffectHandles)
	{
		if (ActiveGEHandle.IsValid())
			ASC->RemoveActiveGameplayEffect(ActiveGEHandle);
	}

	RemoveOnEndEffectHandles.Empty();
}

bool UGA_Base::CheckCooldown(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (const FGameplayTagContainer* CooldownTags = GetCooldownTags())
	{
		if (CooldownTags->Num() > 0)
		{
			const UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
			check(AbilitySystemComponent != nullptr);
			if (AbilitySystemComponent->HasAnyMatchingGameplayTags(*CooldownTags))
			{
				const FGameplayTag& CooldownTag = UAbilitySystemGlobals::Get().ActivateFailCooldownTag;

				if (OptionalRelevantTags && CooldownTag.IsValid())
				{
					OptionalRelevantTags->AddTag(CooldownTag);
				}
				
				if (const AMCharacter* Character = Cast<AMCharacter>(ActorInfo->AvatarActor.Get()))
				{
					Character->OnAbilityFailed.Broadcast(EActivateFailCode::Cooldown);
				}
				
				return false;
			}
		}
	}
	
	return true;
}
