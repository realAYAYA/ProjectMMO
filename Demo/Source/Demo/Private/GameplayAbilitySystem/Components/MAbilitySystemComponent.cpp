// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Components/MAbilitySystemComponent.h"

bool UMAbilitySystemComponent::CanMove() const
{
	TArray<FGameplayTag> StateTags;
	MoveLimitTags.GetGameplayTagArray(StateTags);
	for (const FGameplayTag& Tag : StateTags)
	{
		if (HasMatchingGameplayTag(Tag))
			return false;
	}

	return true;
}

bool UMAbilitySystemComponent::CanCastSpell() const
{
	// 被沉默
	TArray<FGameplayTag> StateTags;
	SilenceTags.GetGameplayTagArray(StateTags);
	for (const FGameplayTag& Tag : StateTags)
	{
		if (HasMatchingGameplayTag(Tag))
			return false;
	}

	// 被昏迷
	StateTags.Empty();
	StunnedTags.GetGameplayTagArray(StateTags);
	for (const FGameplayTag& Tag : StateTags)
	{
		if (HasMatchingGameplayTag(Tag))
			return false;
	}

	return true;
}

bool UMAbilitySystemComponent::CanUseAbility() const
{
	// 被眩晕
	TArray<FGameplayTag> StateTags;
	StunnedTags.GetGameplayTagArray(StateTags);
	for (const FGameplayTag& Tag : StateTags)
	{
		if (HasMatchingGameplayTag(Tag))
			return false;
	}

	return true;
}
