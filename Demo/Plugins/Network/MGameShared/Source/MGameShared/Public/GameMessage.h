// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameDefines.h"

#include "GameMessage.generated.h"

USTRUCT(BlueprintType)
struct FNetworkMessage
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

	TArray<uint8>& SetBody() { return Body; }

	void SerializeToArray(TArray<uint8>& Data)
	{
		FMemoryWriter Writer(Data, false);
		UScriptStruct* DataType = StaticStruct();
		DataType->SerializeTaggedProperties(Writer, (uint8*)this, DataType, nullptr);
	}

	void ParseFromArray(const TArray<uint8>& Data)
	{
		FMemoryReader Reader(Data, false);
		UScriptStruct* DataType = StaticStruct();
		DataType->SerializeTaggedProperties(Reader, (uint8*)this, DataType, nullptr);
	}

private:
	
	UPROPERTY()
	TArray<uint8> Body;
};

USTRUCT(BlueprintType)
struct FGameMessage
{
	GENERATED_USTRUCT_BODY()

public:
	
	virtual uint64 GetTypeID() const { return 0; }

	virtual ~FGameMessage() = default;

	virtual UScriptStruct* RequestStaticStruct() const { return StaticStruct(); }

	void SerializeToArray(TArray<uint8>& Data) const
	{
		FMemoryWriter Writer(Data, false);
		UScriptStruct* DataType = RequestStaticStruct();
		DataType->SerializeTaggedProperties(Writer, (uint8*)this, DataType, nullptr);
	}

	void ParseFromArray(const TArray<uint8>& Data) const
	{
		FMemoryReader Reader(Data, false);
		UScriptStruct* DataType = RequestStaticStruct();
		DataType->SerializeTaggedProperties(Reader, (uint8*)this, DataType, nullptr);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FLoginGameReq : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FString UserName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FString ClientVersion;
	
	virtual uint64 GetTypeID() const override { return KeyTypeID; }
	virtual UScriptStruct* RequestStaticStruct() const override { return StaticStruct(); }

	static constexpr uint64 KeyTypeID = 0;
};

USTRUCT(BlueprintType)
struct FLoginGameAck : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	ELoginGameRetCode Ret = ELoginGameRetCode::UnKnow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	bool bReLogin = false;
	
	virtual uint64 GetTypeID() const override { return KeyTypeID; }
	virtual UScriptStruct* RequestStaticStruct() const override { return StaticStruct(); }

	static constexpr uint64 KeyTypeID = 1;
};
