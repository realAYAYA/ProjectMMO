// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MGameTypes.h"
#include "Net/RpcObject.h"
#include "Inventory.generated.h"

class UItem;

/**
 * 
 */
UCLASS()
class DEMO_API UInventory : public URpcObject
{
	GENERATED_BODY()
	
public:
	
	UInventory(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	const UItem* GetItemByConfigID(const int32 InConfigID) { return nullptr; };

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	const UItem* GetItemByUID(const int32 UID) { return nullptr; };

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	int32 GetItemNum(const int32 InConfigID) { return 0; };

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "UseItem")
	void K2_UseItem(const int32 ID, const FOnRpcResult& InCallback);

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "AddItem")
	void K2_AddItem(const int32 ID, const FOnRpcResult& InCallback);

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "DeleteItem")
	void K2_DeleteItem(const int32 ID, const FOnRpcResult& InCallback);

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "MoveItem")
	void K2_MoveItem(const int32 ID, const FOnRpcResult& InCallback);

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "SplitItem")
	void K2_SplitItem(const int32 ID, const FOnRpcResult& InCallback);

	void LoadData(const FMInventoryData& InData);
	
	void Fill(FMInventoryData& OutData);

private:

	UPROPERTY()
	TMap<int32, UItem*> AllItems;

	UPROPERTY()
	TArray<UItem*> Equipments;

	/** Network & Rpc*/

	UFUNCTION(Server, Reliable, Category = "ProjectM")
	void UseItemReq(const int32 ID);

	UFUNCTION(Client, Reliable, Category = "ProjectM")
	void UseItemAck(const ERpcErrorCode Code);

};
