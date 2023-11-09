// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFwd.h"
#include "GameMessage.generated.h"

////////////////////////////////////////////////DS服务器专用消息类型///////////////////////////////////////////////////////
// 仅用于DS与WebSocket服务器直接通信

USTRUCT()
struct FLoginAsDSReq : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()
	
	UPROPERTY()
	int32 Token = 0;

	UPROPERTY()
	FString ClientVersion;
};

USTRUCT()
struct FLoginAsDSAck : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()

	UPROPERTY()
	ELoginGameRetCode Ret = ELoginGameRetCode::UnKnow;

	UPROPERTY()
	bool bReady = false;
};

USTRUCT()
struct FShutMeDownDSReq : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()
	
	UPROPERTY()
	ELoginGameRetCode Ret = ELoginGameRetCode::UnKnow;

	UPROPERTY()
	bool bReady = false;
};

// Todo 请求关卡进度，保存关卡进度，请求入驻玩家数据，上传玩家数据，玩家离开关卡

//////////////////////////////////////////////////客户端专用消息类型///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FLoginGameReq : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FString Account;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FString ClientVersion;
};

USTRUCT(BlueprintType)
struct FLoginGameAck : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	ELoginGameRetCode Ret = ELoginGameRetCode::UnKnow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	bool bReLogin = false;
};

//////////////////////////////////////////////////客户端专用通知消息类型////////////////////////////////////////////////////
// 用于客户端与WebSocket之间的Notify类型的消息通信

USTRUCT(BlueprintType)
struct FUpdateChatMessage : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FString Text;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FString Name = TEXT("None");
	
	UPROPERTY()
	uint64 UserID = 0;
};
