// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameMessage.h"
#include "GameRpc.generated.h"

/**
 * Network Client Callback
 */

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnRpcResult, ERpcErrorCode, ErrorCode, bool, bOk);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRpcResult2, bool, bOk);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DEMO_API UGameRpc : public UObject
{
	GENERATED_BODY()

public:
	
	/**
	 * 登录游戏
	*/
	
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "LoginGame")
	void K2_LoginGame(const FLoginGameReq& Req, const FOnRpcResult& InCallback);
	
	UFUNCTION(Server, Reliable)
	void LoginGame(const FLoginGameReq& Req);

	UFUNCTION(Client, Reliable)
	void LoginGameResult(const ERpcErrorCode ErrorCode, const FLoginGameAck& Ack);

	/**
	 *  进入世界
	 */
	
	UFUNCTION(BlueprintCallable, Server, Reliable, Reliable, Category = "ProjectM", DisplayName = "EnterLevel")
	void K2_EnterWorld(const int32 InRoleID);

	UFUNCTION(Client, Reliable)
	void EnterWorld(const ERpcErrorCode ErrorCode, const bool bOk);

	UFUNCTION(Client, Reliable)
	void TransportPosition(const ERpcErrorCode ErrorCode, const bool bOk);

private:

	// Rpc回调池，将来自蓝图的回调逻辑缓存，在UE-DS-Rpc函数执行后触发，之后自动销毁，适合那些单次触发的Rpc
	TMap<FString, const FOnRpcResult*> RpcCallbackPool;
};