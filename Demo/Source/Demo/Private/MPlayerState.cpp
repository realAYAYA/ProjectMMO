// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerState.h"

#include "Inventory\Inventory.h"
#include "Net/MGameMessage.h"

AMPlayerState::AMPlayerState()
{
	InventoryModule = NewObject<UInventory>();

	CurrentRoleData = nullptr;
}

int64 AMPlayerState::GetUserID() const
{
	return UserData.UserID;
}

FString AMPlayerState::GetUserName() const
{
	return UserData.UserName;
}

void AMPlayerState::K2_Login(const int64 ID, const FOnLoginResult& InCallback)
{
	OnLoginResult = InCallback;

	Login(ID);
}

void AMPlayerState::Login_Implementation(const int64 ID)
{
	// GetData from SaveGame
}

void AMPlayerState::LoadData_Implementation(const FMUserData& InData)
{
	UserData = InData;
	OnLoginResult.ExecuteIfBound(ELoginCode::Ok);
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
