// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameTypes.h"
#include "GameFramework/PlayerState.h"

#include "MPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API AMPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	AMPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void LoadData(const FRoleData& InData);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ProjectM")
	bool IsOnline() const { return MPlayerID != 0; }

	void SetPlayerID(const uint64 InID) { MPlayerID = InID; }// 仅登录时调用
	uint64 GetPlayerID() const { return MPlayerID; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ProjectM")
	const FRoleData& GetRoleData() const { return RoleData; }
	
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	class UInventory* InventoryModule;// Todo 背包模块
	
	// Todo 天赋模块
	// Todo 任务模块
	// Todo 邮件模块
	// Todo 装扮模块
	// Todo 成就模块 - ToSteam
	// Todo 聊天模块 - 服务器微服务
	// Todo 拍卖行 - 服务器微服务
	
protected:

	virtual void CopyProperties(APlayerState* PlayerState) override;
	
	virtual void Reset() override;

private:

	UPROPERTY()
	FRoleData RoleData;

	UPROPERTY(Replicated)
	uint64 MPlayerID;
};