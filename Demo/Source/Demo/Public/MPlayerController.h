// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "MGameTypes.h"

#include "MPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishedLogin, const ELoginCode, Code);

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

	/** Steam, Epic, PS4 ID*/
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "GetUserID")
	FString K2_GetUserID() const;
	
	/** 不保证在游戏开始（网络同步完成前）时，可以拿到非空的PlayerState*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ProjecetM")
	AMPlayerState* GetMPlayerState() const;

protected:

	virtual void BeginPlay() override;
	
	virtual void OnRep_PlayerState() override;

private:
	
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
	void LoginReq(const FString& InID, const FString& Name);

	UFUNCTION(Client, Reliable)
	void LoginAck(const ELoginCode Code, const FMUserData& InData);

	UFUNCTION(Server, Reliable)
	void CreateRoleReq(const FString& InID, const FCreateRoleParams& InParam);

	UFUNCTION(Client, Reliable)
	void CreateRoleAck(const FRoleData& InData);

	UFUNCTION(Server, Reliable)
	void ChooseRoleReq(const FString& InID, const FString& InName);

	UFUNCTION(Client, Reliable)
	void ChooseRoleAck(const bool bOk);
	
	UPROPERTY()
	TMap<FString, FOnRpcResult> RpcManager;
	
	FOnLoginResult OnLoginCallback;
};
