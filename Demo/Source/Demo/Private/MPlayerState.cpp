// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerState.h"

#include "Inventory\Inventory.h"

AMPlayerState::AMPlayerState()
{
	InventoryModule = NewObject<UInventory>(this);
}

void AMPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

void AMPlayerState::Reset()
{
	Super::Reset();
}
