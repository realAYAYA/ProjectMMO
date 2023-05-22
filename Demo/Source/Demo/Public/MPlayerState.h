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

public:
	UFUNCTION(BlueprintCallable, Category = Test)
	const FString& GetCharacterName() const { return MyName; }

protected:
	UPROPERTY()
	class UInventory* InventoryModule;

	/** 客户端调用，服务器执行*/
	UFUNCTION(BlueprintCallable, Category = ATest, Server, Reliable)
	void Req();

	/** 服务器调用，客户端执行*/
	UFUNCTION(Client, Reliable)
	void Ack(const int32 InData);

	/** Todo Test*/
	UFUNCTION(BlueprintCallable, Category = ATest)
	void SetCharacterName(const FString& InName) {MyName = InName ; };
	
	UPROPERTY()
	FString MyName = "None";
};