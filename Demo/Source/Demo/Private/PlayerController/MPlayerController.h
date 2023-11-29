// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "MGameTypes.h"
#include "MGameCommon.h"
#include "MPlayerController.generated.h"

class AMPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPCNotice, const FString&, NewMessage);

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
	FOnPCNotice OnNotice;

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	AMPlayerState* GetMPlayerState() const;
	
	// 请求角色数据
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "ReqMyRoleData")
	void K2_ReqMyRoleData(const FOnRpcResult& Callback);

	// 请求生成角色
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ProjectM")
	void RequestPawn(const FTransform& SpawnTransform);

private:

	// Rpc implementation
	
	UFUNCTION(Server, Reliable)
	void GetMyRoleDataReq(const uint64 InPlayerID);

	UFUNCTION(Client, Reliable)
	void GetMyRoleDataAck(const FRoleData& InData, EOpErrorCode Error);
	
	UFUNCTION(Client, Unreliable)
	void ShowNotice(const FString& InMessage);


	// Rpc封装
	
	UPROPERTY()
	TMap<FString, FRpcPendingData> RequestPendingData;

	void AddRpcCallback(const FString& In, const FOnRpcResult& InCallback);

	const FOnRpcResult* FindRpcCallback(const FString& In);
	
};
