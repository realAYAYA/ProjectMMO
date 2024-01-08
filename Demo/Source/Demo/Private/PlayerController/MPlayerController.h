// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "MGameTypes.h"
#include "Net/MNetFwd.h"
#include "MPlayerController.generated.h"

class AMPlayerCharacter;
class AMPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStringNotice, const FString&, NewMessage);

/**
 * 项目基类PC
 */
UCLASS()
class AMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	// 收到來自DS的消息通知
	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnStringNotice OnStringNoticeFromDs;
	
	// 收到來自WebSocket的消息通知
	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnStringNotice OnStringNoticeFromWebSocket;

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	AMPlayerState* GetMPlayerState() const;
	
	// 请求角色数据
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "ReqMyRoleData")
	void K2_ReqMyRoleData(const FOnRpcResult& Callback);

	// 请求角色数据
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "RequestPawn")
	void K2_RequestPawn(const FTransform& SpawnTransform);

	// 请求角色实例
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	AMPlayerCharacter* GetMCharacterPawn() const;

private:

	// Rpc implementation
	
	UFUNCTION(Server, Reliable)
	void GetMyRoleDataReq(const uint64 InPlayerID);

	UFUNCTION(Client, Reliable)
	void GetMyRoleDataAck(const FRoleData& InData, EOpErrorCode Error);

	// 请求生成角色
	UFUNCTION(Server, Reliable)
	void RequestPawn(const FTransform& SpawnTransform);
	
	UFUNCTION(Client, Unreliable)
	void ShowNotice(const FString& InMessage);
	
	// Rpc封装
	FRpcManager RpcManager;
	
};
