// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFwd.h"
#include "MGameTypes.h"
#include "GameMessage.generated.h"

////////////////////////////////////////////////DS服务器专用消息类型///////////////////////////////////////////////////////
// 仅用于DS与WebSocket服务器直接通信

// DS进程启动完成后发回消息
USTRUCT()
struct FLoginAsDSReq : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()

	// 验证码
	UPROPERTY()
	uint64 Token = 0;

	// DS进程是否启动成功，并初始化完成
	UPROPERTY()
	bool bSucceed = false;
};

USTRUCT()
struct FLoginAsDSAck : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()

	// DS收到则将自己置为活跃状态，接收进入玩家
	UPROPERTY()
	bool bOk = false;
};

// DS进程请求关闭自己 
USTRUCT()
struct FCloseMeReq : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()
	
};

USTRUCT()
struct FCloseMeAck : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()
	
	UPROPERTY()
	bool bAllow = false;
};

// 拉取玩家数据
USTRUCT()
struct FPullRoleDataReq : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()
	
	UPROPERTY()
	uint64 PlayerID = 0;

	// 需要请求的角色名称
	UPROPERTY()
	FString RoleName;
};

USTRUCT()
struct FPullRoleDataAck : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()
	
	UPROPERTY()
	bool bOk = false;

	// 角色数据
	UPROPERTY()
	FRoleData RoleData;
};

// DS提交玩家数据
USTRUCT()
struct FCommitRoleDataReq : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()

	// 玩家数据，id，角色id
	UPROPERTY()
	uint64 PlayerID = 0;
	
	// 角色数据
	UPROPERTY()
	FRoleData RoleData;
};

USTRUCT()
struct FCommitRoleDataAck : public FGameMessage
{
	GENERATED_USTRUCT_BODY()

	GENERATED_CUSTOM_BODY()
	
	UPROPERTY()
	bool bOk = false;
};

// Todo 请求关卡进度，保存关卡进度，请求入驻玩家数据，，玩家离开关卡

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
	
	UPROPERTY()
	uint64 PlayerID = 0;

	// 角色预览数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	TArray<int32> RolePreviewData;
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
