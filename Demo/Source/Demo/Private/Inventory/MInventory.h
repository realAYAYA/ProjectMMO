// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameMessageFwd.h"
#include "MGameTypes.h"
#include "MInventoryBase.h"

#include "Net/MNetFwd.h"
#include "MInventory.generated.h"

class UMItem;

/**
 * 
 */
UCLASS()
class DEMO_API UMInventory : public UMInventoryBase
{
	GENERATED_BODY()
	
public:
	
	UMInventory(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	const UMItem* GetItemByConfigID(const int32 InConfigID) { return nullptr; };

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	const UMItem* GetItemByUID(const int32 UID) { return nullptr; };

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "UseItem")
	void K2_UseItem(const int32 ID, const FOnRpcResult& InCallback);
	
	void AddItem(const int32 ID, const int32 InNum = 1);

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "DeleteItem")
	void K2_DeleteItem(const int32 ID, const FOnRpcResult& InCallback);

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "MoveItem")
	void K2_MoveItem(const int32 ID, const FOnRpcResult& InCallback);

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "SplitItem")
	void K2_SplitItem(const int32 ID, const FOnRpcResult& InCallback);

	void LoadData(const FInventoryData& InData);
	
	void Fill(FInventoryData& OutData);

	void SendRefreshItemsCacheToMe();
	
private:
	
	/** Network & Rpc*/

	UFUNCTION(Server, Reliable, Category = "ProjectM")
	void UseItemReq(const int32 ID);

	UFUNCTION(Client, Reliable, Category = "ProjectM")
	void UseItemAck(const ERpcErrorCode Code);

	FRpcManager RpcManager;
};
