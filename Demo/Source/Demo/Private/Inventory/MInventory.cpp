// Fill out your copyright notice in the Description page of Project Settings.


#include "MInventory.h"

UMInventory::UMInventory(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void UMInventory::K2_UseItem(const int32 ID, const FOnRpcResult& InCallback)
{
	// Todo 检测背包数据
	//InCallback.ExecuteIfBound(false);
}

void UMInventory::AddItem(const int32 ID, const int32 InNum)
{
	// Todo 检查不进包道具
	// 
}

void UMInventory::K2_DeleteItem(const int32 ID, const FOnRpcResult& InCallback)
{
}

void UMInventory::K2_MoveItem(const int32 ID, const FOnRpcResult& InCallback)
{
}

void UMInventory::K2_SplitItem(const int32 ID, const FOnRpcResult& InCallback)
{
}

void UMInventory::LoadData(const FInventoryData& InData)
{
}

void UMInventory::Fill(FInventoryData& OutData)
{
	
}

void UMInventory::UseItemReq_Implementation(const int32 ID)
{
}

void UMInventory::UseItemAck_Implementation(const ERpcErrorCode Code)
{
	if (true)
	{
		
	}
}