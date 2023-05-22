// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEECLifeSteal.generated.h"

/**
 * 
 */
UCLASS()
class UGEECLifeSteal : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UGEECLifeSteal();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
