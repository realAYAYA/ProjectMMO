// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/Abilities/Base/GA_DirectSkill.h"
#include "GA_CastSpell.generated.h"

/**
 * 
 */
UCLASS()
class UGA_CastSpell : public UGA_DirectSkill
{
	GENERATED_BODY()


public:

	/** 施法时间*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	float CastTime = 2.5f;

	UGA_CastSpell();

	virtual EActivateFailCode CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
private:

	UPROPERTY()
	class UMAbilityTask* SpellTask;
	
};
