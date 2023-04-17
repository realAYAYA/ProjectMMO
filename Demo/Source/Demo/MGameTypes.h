// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameTypes.generated.h"

USTRUCT(BlueprintType)
struct FMyStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayEffect>> Effects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> Abilities;
};
