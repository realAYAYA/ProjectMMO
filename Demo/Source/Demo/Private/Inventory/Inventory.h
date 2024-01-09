// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDefines.h"
#include "MGameTypes.h"

#include "Net/MNetFwd.h"
#include "Inventory.generated.h"

class UItem;


/** 道具数据缓存*/
USTRUCT() 
struct FRefreshItems
{
	GENERATED_USTRUCT_BODY()

	// 添加或更改
	UPROPERTY()
	TArray<FMItemData> AddOrModify;

	// 待删除道具
	UPROPERTY()
	TArray<FMItemData> Junks;

	bool IsEmpty() const
	{
		return !(!AddOrModify.IsEmpty() || !Junks.IsEmpty());
	}
	
	void Clear()
	{
		AddOrModify.Empty();
		Junks.Empty();
	}
};


/**
 * 
 */
UCLASS()
class DEMO_API UInventory : public UObject
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
	
	void AddItem(const int32 ID, const int32 InNum = 1);

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "DeleteItem")
	void K2_DeleteItem(const int32 ID, const FOnRpcResult& InCallback);

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "MoveItem")
	void K2_MoveItem(const int32 ID, const FOnRpcResult& InCallback);

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "SplitItem")
	void K2_SplitItem(const int32 ID, const FOnRpcResult& InCallback);

	void LoadData(const FMInventoryData& InData);
	
	void Fill(FMInventoryData& OutData);

	void SendRefreshItemsCacheToMe();

protected:

	virtual void Tick(float DeltaTime);
	
private:

	int32 GenerateItemUID();

	UPROPERTY()
	TMap<int32, UItem*> AllItems;

	UPROPERTY()
	TArray<UItem*> Equipments;

	int32 SerialNum = 0;
	
	/** Network & Rpc*/
	
	UFUNCTION(Client, Reliable)
	void SendRefreshItemsCacheToMe(const FRefreshItems& Cache);

	FRefreshItems RefreshItemsCache;

	UFUNCTION(Server, Reliable, Category = "ProjectM")
	void UseItemReq(const int32 ID);

	UFUNCTION(Client, Reliable, Category = "ProjectM")
	void UseItemAck(const ERpcErrorCode Code);

	FRpcManager RpcManager;
};
