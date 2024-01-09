// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory.h"

UInventory::UInventory(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void UInventory::K2_UseItem(const int32 ID, const FOnRpcResult& InCallback)
{
	// Todo 检测背包数据
	//InCallback.ExecuteIfBound(false);
}

void UInventory::AddItem(const int32 ID, const int32 InNum)
{
	// Todo 检查不进包道具
	// 
}

void UInventory::K2_DeleteItem(const int32 ID, const FOnRpcResult& InCallback)
{
}

void UInventory::K2_MoveItem(const int32 ID, const FOnRpcResult& InCallback)
{
}

void UInventory::K2_SplitItem(const int32 ID, const FOnRpcResult& InCallback)
{
}

void UInventory::LoadData(const FMInventoryData& InData)
{
}

void UInventory::Fill(FMInventoryData& OutData)
{
	
}

void UInventory::SendRefreshItemsCacheToMe_Implementation(const FRefreshItems& Cache)
{
	// Cache
}

void UInventory::Tick(float DeltaTime)
{
	if (!RefreshItemsCache.IsEmpty())
	{
		SendRefreshItemsCacheToMe();
	}
}

int32 UInventory::GenerateItemUID()
{
	++SerialNum;
	while (SerialNum == 0 || GetItemByUID(SerialNum))
	{
		++SerialNum;
	}

	return SerialNum;
}

void UInventory::SendRefreshItemsCacheToMe()
{
	SendRefreshItemsCacheToMe(RefreshItemsCache);
	RefreshItemsCache.Clear();
}

void UInventory::UseItemReq_Implementation(const int32 ID)
{
}

void UInventory::UseItemAck_Implementation(const ERpcErrorCode Code)
{
	if (true)
	{
		
	}
}