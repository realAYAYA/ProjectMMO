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

	AMPlayerState() {};

	UFUNCTION()
	void RequestRoleData() {}

	UFUNCTION()
	void SaveData() {}
	
	UFUNCTION()
	void Offline() {}

protected:

	// Todo 背包
	UPROPERTY()
	class UInventory* InventoryModule;
	
	
	// Todo 天赋模块
	// Todo 任务模块
	// Todo 邮件模块
	// Todo 装扮模块
	// Todo 成就模块 - ToSteam
	// Todo 聊天模块 - 服务器微服务

	/** 客户端调用，服务器执行*/
	UFUNCTION(BlueprintCallable, Category = ProjectSS, Server, Reliable)
	void Req();

	/** 服务器调用，客户端执行*/
	UFUNCTION(Client, Reliable)
	void Ack(const int32 InData);
	
};