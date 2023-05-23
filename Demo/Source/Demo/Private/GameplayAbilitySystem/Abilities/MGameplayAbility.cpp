// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/MGameplayAbility.h"

#include "GameplayAbilitySystem/Components/MAbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "Characters/MCharacter.h"

bool UMGameplayAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;
	
	UMAbilitySystemComponent* MyASC = Cast<UMAbilitySystemComponent>(ActorInfo->AbilitySystemComponent);
	if (!MyASC)
		return false;
	AMCharacter* Target = Cast<AMCharacter>(ActorInfo->AvatarActor);
	if (Target)
		return false;
	UMAbilitySystemComponent* TargetASC = Cast<UMAbilitySystemComponent>(Target->GetAbilitySystemComponent());
	if (!TargetASC)
		return false;

	// Todo 消耗品

	// Todo 是否是友军 Friendly，阵营
	
	return true;
}

void UMGameplayAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);

	// Todo 对目标施加效果
	//MyASC->ApplyGameplayEffectToTarget(EffectToEnemy[0], TargetASC)
	// Todo 对自己施加效果
	//AbilitySystemComponent->ApplyGameplayEffectToSelf();
	
	UAbilitySystemComponent* AbilitySystemComponent = OwnerInfo->AbilitySystemComponent.Get();
	if (!AbilitySystemComponent)
		return;

	const FGameplayEffectContextHandle EffectContextHandle = OwnerInfo->AbilitySystemComponent->MakeEffectContext();

	for (TSubclassOf<UGameplayEffect>& GameplayEffect : OngoingEffectsToJustApplyOnStart)
	{
		if (!GameplayEffect.Get())
			continue;
		
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContextHandle);
		if (!SpecHandle.IsValid())
			continue;

		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		if (!ActiveGEHandle.WasSuccessfullyApplied())
			ABILITY_LOG(Log, TEXT("Ability %s faild to apply startup effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
	}

	if (!this->IsInstantiated())
		return;

	for (TSubclassOf<UGameplayEffect>& GameplayEffect : OngoingEffectsToRemoveOnEnd)
	{
		if (!GameplayEffect.Get())
			continue;

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContextHandle);
		if (!SpecHandle.IsValid())
			continue;
		
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		if (!ActiveGEHandle.WasSuccessfullyApplied())
		{
			ABILITY_LOG(Log, TEXT("Ability %s faild to apply startup effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
		}
		else
			RemoveOnEndEffectHandles.Add(ActiveGEHandle);
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

AMCharacter* UMGameplayAbility::GetMCharacterFromActorInfo() const
{
	return Cast<AMCharacter>(GetAvatarActorFromActorInfo());
}
