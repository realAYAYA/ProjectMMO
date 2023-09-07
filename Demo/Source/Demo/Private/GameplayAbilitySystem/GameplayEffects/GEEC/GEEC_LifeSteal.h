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
class UMGameplayEffectExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UMGameplayEffectExecutionCalculation();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	static AMCharacter* GetTarget(const FGameplayEffectCustomExecutionParameters& ExecutionParams);

	static AMCharacter* GetSource(const FGameplayEffectCustomExecutionParameters& ExecutionParams);
};
