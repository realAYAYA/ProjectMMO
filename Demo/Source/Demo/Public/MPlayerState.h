// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "MGameTypes.h"

#include "MPlayerState.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRpcResult, bool, bOk);

/**
 * 
 */
UCLASS()
class DEMO_API AMPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	AMPlayerState();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ProjectM")
	bool IsOnline() const { return UserData.UserID > 0; }

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	int64 GetUserID() const { return UserID; }

	// Steam, Epic, PS4 : ID, to show
	UPROPERTY(Replicated)
	uint64 UserID;

	// Steam, Epic, PS4 : Name, to show
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "ProjectM")
	FString UserName;

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	int32 GetRoleNum() const;

	/** Login*/
public:

	//typedef TFunction<void(const ELoginCode Code)> FOnLoginResult;
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void Login(const FOnRpcResult& InCallback);

	UFUNCTION(Server, Reliable)
	void LoginReq(const uint64 ID, const FString& Name);

	UFUNCTION(Client, Reliable)
	void LoginAck(const FMUserData& InData);

private:
	
	//FOnLoginResult OnLoginResult;

	/** Create Role*/
public:
	
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void K2_CreateRole(const FCreateRoleParams& InParam, const FOnRpcResult& InCallback);

	UFUNCTION(Server, Reliable)
	void CreateRoleReq(const int32 InID, const FCreateRoleParams& InParam);

	UFUNCTION(Client, Reliable)
	void CreateRoleAck(const FMUserData& InData);

	/** Change Role*/
public:

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "ChangeRole")
	void K2_ChangeRole(const FString& InName, const FOnRpcResult& InCallback);

	UFUNCTION(Server, Reliable)
	void ChangeRole(const int32 InID, const FString& InName);

	UFUNCTION(Client, Reliable)
	void ChangeRoleResult(const bool bOk);
	
	void SaveData();

	const FMUserData& GetUserData() const { return UserData; }
	
protected:

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

	virtual void CopyProperties(APlayerState* PlayerState) override;
	
	virtual void Reset() override;

private:

	UPROPERTY()
	FMUserData UserData;
	
	UPROPERTY()
	TMap<FString, FOnRpcResult> RpcManager;
};