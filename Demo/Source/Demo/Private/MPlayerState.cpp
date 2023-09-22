// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerState.h"

#include "Demo.h"
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
	// 如果没有用户就为其注册一个，并返回数据

	UE_LOG(LogProjectM, Log, TEXT("Server: Someone connected"));

	//LoginResult(); // RpcToClient
}

void AMPlayerState::LoginResult_Implementation(const FMUserData& InData)
{
	UserData = InData;
	// Todo Setup Data
	
	
	bool bOk = OnLoginResult.ExecuteIfBound(ELoginCode::Ok);
}

void AMPlayerState::K2_CreateRole(const FCreateUserParams& InParam, const FOnRpcResult& InCallback)
{
	RpcManager.Add(TEXT("CreateRole"), InCallback);
	CreateRole(InParam);
}

void AMPlayerState::CreateRole_Implementation(const FCreateUserParams& InParam)
{
	
	CreateRoleResult(true);
}

void AMPlayerState::CreateRoleResult_Implementation(const FMUserData& InData)
{
	
	if (const FOnRpcResult* Callback = RpcManager.Find(TEXT("CreateRole")))
	{
		if (InData.UserID == 0)
			return;
		bool bOk = Callback->ExecuteIfBound(true);
	}
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
