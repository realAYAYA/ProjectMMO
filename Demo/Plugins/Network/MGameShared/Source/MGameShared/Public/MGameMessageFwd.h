// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MGameMessageFwd.generated.h"

UENUM(BlueprintType)
enum class ERpcErrorCode : uint8
{
	Ok				UMETA(DisplayName = "Ok"),
	UnKnow			UMETA(DisplayName = "UnKnow"),
	Unimplemented	UMETA(DisplayName = "Unimplemented"),
	TimeOut			UMETA(DisplayName = "TimeOut"),
};

UENUM()
enum class ERpcMessageOp : uint8
{
	Notify			UMETA(DisplayName = "Notify"),
	Request			UMETA(DisplayName = "Request"),
	Response		UMETA(DisplayName = "Response"),
};

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
	ERpcMessageOp RpcMessageOp = ERpcMessageOp::Request;

	UPROPERTY()
	int64 Timestamp = 0;

	const TArray<uint8>& GetBody() const { return Body; }

	void SetBody(const TArray<uint8>& InData) { Body = InData; }

	TArray<uint8>& SetBody() { return Body; }

	void SerializeToArray(TArray<uint8>& Data)
	{
		FMemoryWriter Writer(Data);
		UScriptStruct* DataType = StaticStruct();
		DataType->SerializeTaggedProperties(Writer, (uint8*)this, DataType, nullptr);
	}

	void ParseFromArray(const TArray<uint8>& Data)
	{
		FMemoryReader Reader(Data);
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


#define GENERATED_CUSTOM_BODY() \
virtual uint64 GetTypeID() const override { return KeyTypeID; } \
virtual UScriptStruct* RequestStaticStruct() const override { return StaticStruct(); } \
static constexpr uint64 KeyTypeID = __LINE__;