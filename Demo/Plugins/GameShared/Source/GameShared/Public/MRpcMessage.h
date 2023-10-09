// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MGameDefines.h"
#include "MGameMessage.h"

#include "MRpcMessage.generated.h"

USTRUCT(BlueprintType)
struct FMGameRpcMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	ERpcErrorCode RpcErrorCode = ERpcErrorCode::Unimplemented;
	
	UPROPERTY()
	uint64 TypeID = 0;
	
	UPROPERTY()
	uint64 SerialNum = 0;

	UPROPERTY()
	ERpcMessageOp RpcMessageOp = ERpcMessageOp::Notify;

	UPROPERTY()
	int64 Timestamp = 0;

	const TArray<uint8>& GetBody() const { return Body; }

	void SetBody(const TArray<uint8>& InData) { Body = InData; }

private:
	
	UPROPERTY()
	TArray<uint8> Body;
};

USTRUCT(BlueprintType)
struct FMGameMessage
{
	GENERATED_USTRUCT_BODY()
	
	virtual uint64 GetTypeID() const { return 0; }

	virtual ~FMGameMessage() = default;
};

USTRUCT(BlueprintType)
struct FLoginReq : public FMGameMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "ProjectM")
	FString UserName;

	UPROPERTY(EditAnywhere, Category = "ProjectM")
	FString ClientVersion;
	
	virtual uint64 GetTypeID() const override
	{
		return KeyTypeID;
	}

private:
	
	static constexpr uint64 KeyTypeID = 0;
};

USTRUCT(BlueprintType)
struct FLoginAck : public FMGameMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "ProjectM")
	ELoginGameRetCode Ret = ELoginGameRetCode::UnKnow;

	UPROPERTY(EditAnywhere, Category = "ProjectM")
	bool bReLogin = false;
	
	virtual uint64 GetTypeID() const override
	{
		return KeyTypeID;
	}

private:
	
	static constexpr uint64 KeyTypeID = 1;
};