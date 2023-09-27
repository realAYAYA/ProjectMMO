// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory.h"

UInventory::UInventory(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void UInventory::K2_UseItem(const int32 ID, const FOnRpcResult& InCallback)
{
	// Todo 检测背包数据
	RpcManager.Add(TEXT("UseItem"), InCallback);
	//InCallback.ExecuteIfBound(false);
}

void UInventory::K2_AddItem(const int32 ID, const FOnRpcResult& InCallback)
{
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

void UInventory::UseItemReq_Implementation(const int32 ID)
{
}

void UInventory::UseItemAck_Implementation(const ERpcErrorCode Code)
{
	if (true)
	{
		
	}
}