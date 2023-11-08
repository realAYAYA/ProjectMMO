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

////////////////////////////////////////////////DS服务器专用消息类型///////////////////////////////////////////////////////
// TypeID 100000打头
// 仅用于DS与WebSocket服务器直接通信

USTRUCT()
struct FLoginAsDSReq : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 Token = 0;

	UPROPERTY()
	FString ClientVersion;
	
	virtual uint64 GetTypeID() const override { return KeyTypeID; }
	virtual UScriptStruct* RequestStaticStruct() const override { return StaticStruct(); }

	static constexpr uint64 KeyTypeID = 100001;
};

USTRUCT()
struct FLoginAsDSAck : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	ELoginGameRetCode Ret = ELoginGameRetCode::UnKnow;

	UPROPERTY()
	bool bReady = false;
	
	virtual uint64 GetTypeID() const override { return KeyTypeID; }
	virtual UScriptStruct* RequestStaticStruct() const override { return StaticStruct(); }

	static constexpr uint64 KeyTypeID = 100002;
};

USTRUCT()
struct FShutMeDownDSReq : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	ELoginGameRetCode Ret = ELoginGameRetCode::UnKnow;

	UPROPERTY()
	bool bReady = false;
	
	virtual uint64 GetTypeID() const override { return KeyTypeID; }
	virtual UScriptStruct* RequestStaticStruct() const override { return StaticStruct(); }

	static constexpr uint64 KeyTypeID = 100003;
};

// Todo 请求关卡进度，保存关卡进度，请求入驻玩家数据，上传玩家数据，玩家离开关卡

//////////////////////////////////////////////////客户端专用消息类型///////////////////////////////////////////////////////

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

	static constexpr uint64 KeyTypeID = 1;
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

	static constexpr uint64 KeyTypeID = 2;
};

//////////////////////////////////////////////////客户端专用通知消息类型////////////////////////////////////////////////////
// TypeID 10000打头
// 用于客户端与WebSocket之间的Notify类型的消息通信

USTRUCT(BlueprintType)
struct FUpdateChatMessage : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FString Text;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FString Name = TEXT("None");
	
	UPROPERTY()
	uint64 UserID = 0;
	
	virtual uint64 GetTypeID() const override { return KeyTypeID; }
	virtual UScriptStruct* RequestStaticStruct() const override { return StaticStruct(); }

	static constexpr uint64 KeyTypeID = 10001;
};
