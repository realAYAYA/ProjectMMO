// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityEnd);

class AMCharacter;

/**
 * 
 */
UCLASS()
class UMAbilityTask : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnAbilityEnd OnAbilityEnd;
};
