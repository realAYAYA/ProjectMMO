// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerState.h"

#include "Demo.h"
#include "MGameInstance.h"
#include "Developer/MSaveGame.h"
#include "Inventory/Inventory.h"
#include "Net/MGameMessage.h"
#include "Net/UnrealNetwork.h"

AMPlayerState::AMPlayerState()
{
	InventoryModule = NewObject<UInventory>();

	CurrentRoleData = nullptr;

	bReplicates = true;
}

int64 AMPlayerState::GetUserID() const
{
	return UserData.UserID;
}

FString AMPlayerState::GetRoleName() const
{
	if (CurrentRoleData)
		return CurrentRoleData->RoleName;
	return TEXT("None");
}

void AMPlayerState::K2_Login(const int64 ID, const FString& Name, const FOnLoginResult& InCallback)
{
	OnLoginResult = InCallback;

	Login(ID, Name);
}

void AMPlayerState::Login_Implementation(const int64 ID, const FString& Name)
{
	// GetData from SaveGame
	// 如果没有用户就为其注册一个，并返回数据
	const UMGameInstance* GameInstance = Cast<UMGameInstance>(GetGameInstance());
	if (GameInstance && GameInstance->SaveGame)
	{
		UMSaveGame* SaveGame = GameInstance->SaveGame;
		if (const FMUserData* FoundData = SaveGame->FindUserDataRef(ID))
		{
			LoginResult(*FoundData);
		}
		else
		{
			// Create User
			FMUserData NewData;
			NewData.UserID = ID;
			NewData.UserName = Name;
			NewData.CreateDate = FDateTime::Now().GetTicks();
			
			if (SaveGame->CreateUser(NewData))
				LoginResult(NewData);
			else
			{
				LoginResult(FMUserData());
			}
		}
	}

	UE_LOG(LogProjectM, Log, TEXT("Server: User: %s enter the game"), *Name);
}

void AMPlayerState::LoginResult_Implementation(const FMUserData& InData)
{
	UserData = InData;
	
	ELoginCode Code = ELoginCode::Ok;
	if (UserData.UserID == 0)
		Code = ELoginCode::Unknown;
	if (UserData.RoleData.Num() <= 0)
		Code = ELoginCode::NoRole;
	
	bool bOk = OnLoginResult.ExecuteIfBound(Code);
}

void AMPlayerState::K2_CreateRole(const FCreateRoleParams& InParam, const FOnRpcResult& InCallback)
{
	RpcManager.Add(TEXT("CreateRole"), InCallback);
	CreateRole(UserData.UserID, InParam);
}

void AMPlayerState::K2_ChangeRole(const FString& InName, const FOnRpcResult& InCallback)
{
	RpcManager.Add(TEXT("ChangeRole"), InCallback);
	ChangeRole(GetUserID(), InName);
}

void AMPlayerState::ChangeRole_Implementation(const int32 UserID, const FString& InName)
{
	
}

void AMPlayerState::ChangeRoleResult_Implementation(const bool bOk)
{
	if (const FOnRpcResult* Callback = RpcManager.Find(TEXT("ChangeRole")))
	{
		Callback->ExecuteIfBound(bOk);
		CurrentRoleData;
	}
}

void AMPlayerState::CreateRole_Implementation(const int32 UserID, const FCreateRoleParams& InParam)
{
	// Todo 创建角色存档
	const UMGameInstance* GameInstance = Cast<UMGameInstance>(GetGameInstance());
	if (GameInstance && GameInstance->SaveGame)
	{
		UMSaveGame* SaveGame = GameInstance->SaveGame;
		if (SaveGame->CreateRole(UserID, InParam))
			CreateRoleResult(SaveGame->FindUserData(UserID));// 返回玩家存档
		else
		{
			CreateRoleResult(FMUserData());
		}

		return;
	}

	CreateRoleResult(FMUserData());
}

void AMPlayerState::CreateRoleResult_Implementation(const FMUserData& InData)
{
	
	if (const FOnRpcResult* Callback = RpcManager.Find(TEXT("CreateRole")))
	{
		bool bOk = InData.UserID != 0;
		if (bOk)
		{
			UserData = InData;
			CurrentRoleData = &UserData.RoleData[0];
			RoleName = CurrentRoleData->RoleName;
		}
		
		bOk = Callback->ExecuteIfBound(bOk);
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

void AMPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(AMPlayerState, RoleName, SharedParams);
}
