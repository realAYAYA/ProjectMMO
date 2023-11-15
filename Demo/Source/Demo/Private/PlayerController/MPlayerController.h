// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "MGameTypes.h"
#include "MGameCommon.h"
#include "MPlayerController.generated.h"

class AMPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPCNotice, const FString&, NewMessage);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLoginResult, const ELoginGameRetCode, RetCode);// Todo 角色预览数据

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
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "Login")
	void K2_Login(const FString& Account, const FOnLoginResult& Callback);
	
	// 请求角色数据
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "ReqMyRoleData")
	void K2_ReqMyRoleData(const FString& RoleName, const FOnRpcResult& Callback);

private:

	// Rpc implementation
	
	UFUNCTION(Client, Reliable)
	void GetMyRoleDataReq(const FString& RoleName);

	UFUNCTION(Server, Reliable)
	void GetMyRoleDataAck(const FRoleData& InData, EOpErrorCode Error);
	
	UFUNCTION(Server, Unreliable)
	void ShowNotice(const FString& InMessage);

	UPROPERTY()
	TMap<FString, FOnRpcResult> RequestPendingData;
	
};
