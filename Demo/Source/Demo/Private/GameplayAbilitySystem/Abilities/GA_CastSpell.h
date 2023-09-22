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
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	int32 Level = 1;

	/** 施法时间*/
	UPROPERTY(EditDefaultsOnly, Category = "CastSpell")
	float CastTime = 2.5f;

	/** 对目标施加效果*/
	UPROPERTY(EditDefaultsOnly, Category = "CastSpell")
	TArray<TSubclassOf<UMGameplayEffect>> EffectsToTarget;

	/** 技能目标类型*/
	UPROPERTY(EditDefaultsOnly, Category = "CastSpell")
	ETargetType TargetType;

	UGA_CastSpell();

	virtual EActivateFailCode CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
private:

	UPROPERTY()
	class UMAbilityTask* SpellTask;
	
};
