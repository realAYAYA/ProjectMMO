// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GMMC_DirectDamage.generated.h"

/**
 * 
 */
UCLASS()
class UGMMC_DirectDamage : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:

	UGMMC_DirectDamage();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	
	FGameplayEffectAttributeCaptureDefinition ManaDef;

	FGameplayEffectAttributeCaptureDefinition MaxManaDef;
};
