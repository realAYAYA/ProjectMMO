// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "MGameTypes.h"

#include "MPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishedLogin, bool, bOk);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRpcResult, bool, bOk);

class AMPlayerState;
/**
 * 
 */
UCLASS()
class DEMO_API AMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ProjecetM")
	bool IsOnline() const;
	
	/** 不保证在游戏开始（网络同步完成前）时，可以拿到非空的PlayerState*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ProjecetM")
	AMPlayerState* GetMPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	int64 GetUserID() const { return UserID; }

	// Steam, Epic, PS4 : ID, to show
	UPROPERTY(Replicated)
	uint64 UserID;

	// Steam, Epic, PS4 : Name, to show
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "ProjectM")
	FString UserName;

protected:

	virtual void BeginPlay() override;
	
	virtual void OnRep_PlayerState() override;

private:

	// Todo 背包模块
	UPROPERTY()
	class UInventory* InventoryModule;
	
	// Todo 天赋模块
	// Todo 任务模块
	// Todo 邮件模块
	// Todo 装扮模块
	// Todo 成就模块 - ToSteam
	// Todo 聊天模块 - 服务器微服务
	// Todo 拍卖行 - 服务器微服务

	
	/** Rpc*/
public:

	/** Todo 手动触发登录，仅供蓝图测试*/
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "Login")
	void K2_Login();
	
	/** Create Role*/
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "CreateRole")
	void K2_CreateRole(const FCreateRoleParams& InParam, const FOnRpcResult& InCallback);

	/** Choose Role*/
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "ChooseRole")
	void K2_ChooseRole(const FString& InName, const FOnRpcResult& InCallback);

	UPROPERTY(BlueprintAssignable)
	FOnFinishedLogin OnFinishedLogin;
	
protected:

private:

	typedef TFunction<void(const ELoginCode Code)> FOnLoginResult;
	void Login(const FOnLoginResult& InCallback);

	UFUNCTION(Server, Reliable)
	void LoginReq(const uint64 ID, const FString& Name);

	UFUNCTION(Client, Reliable)
	void LoginAck(const FMUserData& InData);

	UFUNCTION(Server, Reliable)
	void CreateRoleReq(const uint64 InID, const FCreateRoleParams& InParam);

	UFUNCTION(Client, Reliable)
	void CreateRoleAck(const FRoleData& InData);

	UFUNCTION(Server, Reliable)
	void ChooseRoleReq(const uint64 InID, const FString& InName);

	UFUNCTION(Client, Reliable)
	void ChooseRoleAck(const bool bOk);
	
	UPROPERTY()
	TMap<FString, FOnRpcResult> RpcManager;
	
	FOnLoginResult OnLoginCallback;
};
