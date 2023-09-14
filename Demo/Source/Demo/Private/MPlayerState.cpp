// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerState.h"

#include "Inventory\Inventory.h"

AMPlayerState::AMPlayerState()
{
	InventoryModule = NewObject<UInventory>();
}

void AMPlayerState::LoadData(const FMUserData& InData)
{
	UserData = InData;
}

void AMPlayerState::SaveData()
{
}

void AMPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

void AMPlayerState::Reset()
{
	Super::Reset();
}
