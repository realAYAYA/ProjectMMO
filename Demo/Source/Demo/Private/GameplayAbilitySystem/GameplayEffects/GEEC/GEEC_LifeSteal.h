// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_LifeSteal.generated.h"

class AMCharacter;

/**
 * 
 */
UCLASS()
class UGEEC_LifeSteal : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UGEEC_LifeSteal();

	AMCharacter* GetTarget(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;

	AMCharacter* GetSource(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
