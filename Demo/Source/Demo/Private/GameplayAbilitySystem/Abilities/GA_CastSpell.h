// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/Abilities/MGameplayAbility.h"
#include "GA_CastSpell.generated.h"

/**
 * 
 */
UCLASS()
class UGA_CastSpell : public UMGameplayAbility
{
	GENERATED_BODY()


public:

	/** 技能等级*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectSS")
	int32 Level = 1;

	/** 施法时间*/
	UPROPERTY(EditDefaultsOnly, Category = "CastSpell")
	float CastTime = 2.5f;

	/** 对目标施加效果*/
	UPROPERTY(EditDefaultsOnly, Category = "CastSpell")
	TSubclassOf<UMGameplayEffect> EffectToTarget;

	/** 技能目标类型*/
	UPROPERTY(EditDefaultsOnly, Category = "CastSpell")
	ETargetType TargetType;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

};
