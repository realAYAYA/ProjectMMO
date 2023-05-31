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
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;
	
	return CanActivateCondition(ActorInfo);
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

	// 根据目标类型选择目标
	const AMCharacter* Caster = Cast<AMCharacter>(OwnerInfo->AvatarActor);
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
		if (UAbilitySystemComponent* TargetComponent = Target->GetCurrentTarget()->GetAbilitySystemComponent())
		{
			const FGameplayEffectSpecHandle SpecHandle = TargetComponent->MakeOutgoingSpec(EffectToTarget, Level, EffectContext);
			if (!SpecHandle.IsValid())
			{
				const FActiveGameplayEffectHandle ActiveGEHandle = TargetComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				if (!ActiveGEHandle.WasSuccessfullyApplied())
					ABILITY_LOG(Log, TEXT("Ability %s faild to apply Effect to Target %s"), *GetName(), *GetNameSafe(EffectToTarget));
			}
		}
	}

	// 处理技能开始时施加效果
	for (TSubclassOf<UGameplayEffect>& GameplayEffect : OngoingEffectsToJustApplyOnStart)
	{
		if (!GameplayEffect.Get())
			continue;
		
		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, Level, EffectContext);
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

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, Level, EffectContext);
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

bool UMGameplayAbility::CanActivateCondition(const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (!Cast<UMAbilitySystemComponent>(ActorInfo->AbilitySystemComponent))
		return false;
	
	const AMCharacter* Caster = Cast<AMCharacter>(ActorInfo->AvatarActor);
	
	if (TargetType != ETargetType::Self)
	{
		if (TargetType == ETargetType::Hostile && !Caster->GetCurrentTarget())
			return false;
		
		// Out of range
		const float Distance = (Caster->GetActorLocation() - Caster->GetCurrentTarget()->GetActorLocation()).Length();
		if (Distance > Range || Distance < MinRange)
			return false;
	}
	
	// Todo 消耗品

	// Todo 如果目标类型为Hostile，但是当前目标不是敌对，失败

	return true;
}

AMCharacter* UMGameplayAbility::GetMCharacterFromActorInfo() const
{
	return Cast<AMCharacter>(GetAvatarActorFromActorInfo());
}
