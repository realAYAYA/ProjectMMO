// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/Abilities/MGameplayAbility.h"
#include "GA_Melee.generated.h"

/**
 * 
 */
UCLASS()
class UGA_Melee : public UMGameplayAbility
{
	GENERATED_BODY()

public:
	
	/** 技能等级*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectSS")
	int32 Level = 1;

	/** 怒气消耗*/
	UPROPERTY(EditDefaultsOnly, Category = "CastMelee")
	int32 Rage = 0;

	/** 能量消耗*/
	UPROPERTY(EditDefaultsOnly, Category = "CastMelee")
	int32 Energy = 0;

	/** 对目标施加效果*/
	UPROPERTY(EditDefaultsOnly, Category = "CastMelee")
	TArray<TSubclassOf<UMGameplayEffect>> EffectsToTarget;

	UGA_Melee();

	virtual EActivateFailCode CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
