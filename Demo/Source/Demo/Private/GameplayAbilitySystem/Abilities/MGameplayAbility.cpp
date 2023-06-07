// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/MGameplayAbility.h"

#include "AbilitySystemLog.h"

#include "Characters/MCharacter.h"
#include "GameplayAbilitySystem/Components/MAbilitySystemComponent.h"
#include "GameplayAbilitySystem/GameplayEffects/MGameplayEffect.h"

bool UMGameplayAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	const AMCharacter* Character = Cast<AMCharacter>(ActorInfo->AvatarActor.Get());
	if (!Character || !Cast<UMAbilitySystemComponent>(Character->GetAbilitySystemComponent()))
		return false;

	if (!CheckCooldown(this->CurrentSpecHandle, ActorInfo))
		Character->OnAbilityFailed.Broadcast(EActivateFailCode::Cooldown);
	
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;
	
	return CanActivateCondition(*ActorInfo) == EActivateFailCode::Success;
}

void UMGameplayAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
	
	UAbilitySystemComponent* AbilitySystemComponent = OwnerInfo->AbilitySystemComponent.Get();
	if (!AbilitySystemComponent)
		return;

	const FGameplayEffectContextHandle EffectContext = OwnerInfo->AbilitySystemComponent->MakeEffectContext();

	// 处理技能开始时施加效果
	for (TSubclassOf<UGameplayEffect>& GameplayEffect : OngoingEffectsToJustApplyOnStart)
	{
		if (!GameplayEffect.Get())
			continue;
		
		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (!SpecHandle.IsValid())
			continue;

		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
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

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (!SpecHandle.IsValid())
			continue;
		
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		if (ActiveGEHandle.WasSuccessfullyApplied())
			RemoveOnEndEffectHandles.Add(ActiveGEHandle);
		else
			ABILITY_LOG(Log, TEXT("Ability %s faild to apply startup effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
	}
}

void UMGameplayAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	if (IsInstantiated())
	{
		for (FActiveGameplayEffectHandle& ActiveGEHandle : RemoveOnEndEffectHandles)
		{
			if (ActiveGEHandle.IsValid())
				ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveGEHandle);
		}

		RemoveOnEndEffectHandles.Empty();
	}
		
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

EActivateFailCode UMGameplayAbility::CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const
{
	return EActivateFailCode::Success;
}

AMCharacter* UMGameplayAbility::GetMCharacterFromActorInfo() const
{
	return Cast<AMCharacter>(GetAvatarActorFromActorInfo());
}

UMAbilitySystemComponent* UMGameplayAbility::GetMAbilitySystemComponent() const
{
	if (const AMCharacter* Character = Cast<AMCharacter>(GetAvatarActorFromActorInfo()))
		return Cast<UMAbilitySystemComponent>(Character->GetAbilitySystemComponent());

	return nullptr;
}
