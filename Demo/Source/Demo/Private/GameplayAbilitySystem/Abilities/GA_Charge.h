// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/Abilities/MGameplayAbility.h"
#include "GA_Charge.generated.h"

class UMAbilityTask;

/**
 * 
 */
UCLASS()
class UGA_Charge : public UMGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Charge();

	/** 最小距离*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	int32 MinRange = 400;

	/** 对目标施加效果*/
	UPROPERTY(EditDefaultsOnly, Category = "CastMelee")
	TArray<TSubclassOf<UMGameplayEffect>> EffectsToTarget;

	virtual EActivateFailCode CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	UPROPERTY()
	UMAbilityTask* ChargeTask;
};
