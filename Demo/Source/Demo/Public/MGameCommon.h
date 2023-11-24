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
	Timeout			UMETA(DisplayName = "Timeout"),
	UnKnow			UMETA(DisplayName = "UnKnow"),
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRpcResult, const EOpErrorCode, ErrorCode);

USTRUCT()
struct FRpcPendingData
{
	GENERATED_BODY()
		
	FOnRpcResult Callback;

	FDateTime BeginDate;

	int32 TimeoutSetting;

	FRpcPendingData()
	{
		TimeoutSetting = 3;
	}
	
	explicit FRpcPendingData(const FOnRpcResult& InCallback, const int32 InTimeoutSetting = 3)
	{
		Callback = InCallback;
		BeginDate = FDateTime::UtcNow();
		TimeoutSetting = InTimeoutSetting;
	}

	bool IsTimeOut() const
	{
		return (FDateTime::UtcNow() - BeginDate).GetTotalSeconds() >TimeoutSetting;
	}
};

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
