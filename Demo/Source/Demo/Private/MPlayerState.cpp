// Fill out your copyright notice in the Description page of Project Settings.

#include "MPlayerState.h"
#include "MGameInstance.h"

#include "Inventory/Inventory.h"
#include "Net/UnrealNetwork.h"

AMPlayerState::AMPlayerState(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	InventoryModule = NewObject<UInventory>();
}

int32 AMPlayerState::GetRoleNum() const
{
	return UserData.RoleData.Num();
}

void AMPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

void AMPlayerState::Reset()
{
	Super::Reset();
}

void AMPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(AMPlayerState, UserID, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AMPlayerState, UserName, SharedParams);

	//SharedParams.Condition = COND_InitialOnly;
}