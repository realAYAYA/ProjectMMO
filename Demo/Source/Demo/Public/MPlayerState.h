// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "MGameTypes.h"

#include "MPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API AMPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	AMPlayerState();

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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ProjectM")
	bool IsOnline() const { return UserData.UserID > 0; }

	uint64 GetUserID() const { return UserData.UserID; }
	
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	int32 GetRoleNum() const;
	
	const FMUserData& GetUserData() const { return UserData; }

	void SetUserData(const FMUserData& InData) { UserData = InData; }
	
protected:

	virtual void CopyProperties(APlayerState* PlayerState) override;
	
	virtual void Reset() override;

private:

	UPROPERTY()
	FMUserData UserData;
};