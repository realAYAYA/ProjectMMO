// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UGameplayAbility;
class UGameplayEffect;

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

struct FRpcPendingData
{
	FOnRpcResult Callback;

	FDateTime BeginDate;

	int32 TimeoutSetting;

	FRpcPendingData()
	{
		TimeoutSetting = 10;
	}
	
	explicit FRpcPendingData(const FOnRpcResult& InCallback, const int32 InTimeoutSetting = 3)
	{
		Callback = InCallback;
		BeginDate = FDateTime::UtcNow();
		TimeoutSetting = InTimeoutSetting;
	}

	bool IsTimeOut() const
	{
#if WITH_EDITOR
		return false;// 编辑器模式下没有超时
#else
		return (FDateTime::UtcNow() - BeginDate).GetTotalSeconds() >TimeoutSetting;
#endif
	}
};


/**
 * Rpc封装
 */
class DEMO_API FRpcManager
{

public:

	uint64 AddRpcCallback(const FOnRpcResult& InCallback);
	const FOnRpcResult* FindRpcCallback(const uint64 In);
	void RemoveRpcCallback(const uint64 In);

	void AddRpcCallback(const FString& In, const FOnRpcResult& InCallback);
	const FOnRpcResult* FindRpcCallback(const FString& In);
	void RemoveRpcCallback(const FString& In);

private:
	
	TMap<FString, FRpcPendingData> RequestPendingDataByString;

	TMap<uint64, FRpcPendingData> RequestPendingData;
	uint64 SerialNum = 0;
};

