// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MGameCommon.generated.h"

UENUM(BlueprintType)
enum class EOpErrorCode : uint8
{
	Ok				UMETA(DisplayName = "Ok"),
	Waiting			UMETA(DisplayName = "Waiting"),
	BadParams		UMETA(DisplayName = "BadParams"),
	BadConfig		UMETA(DisplayName = "BadConfig"),
	BadServer		UMETA(DisplayName = "BadServer"),
	BadMemory		UMETA(DisplayName = "BadMemory"),
	UnKnow			UMETA(DisplayName = "UnKnow"),
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRpcResult, const EOpErrorCode, ErrorCode);

// Todo Delete
USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TArray<TSubclassOf<class UGameplayEffect>> Effects;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> Abilities;
};
