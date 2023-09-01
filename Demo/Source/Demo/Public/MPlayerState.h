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

	AMPlayerState();

public:
	
	void LoadData(const FMUserData& InData) { UserData = InData; }

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

	virtual void CopyProperties(APlayerState* PlayerState) override;
	
	virtual void Reset() override;

private:

	FMUserData UserData;
	
};