// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/Abilities/Base/GA_DirectSkill.h"
#include "GA_Melee.generated.h"

/**
 * 
 */
UCLASS()
class UGA_Melee : public UGA_DirectSkill
{
	GENERATED_BODY()

public:

	UGA_Melee();

	virtual EActivateFailCode CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const override;
	
};
