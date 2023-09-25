// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerState.h"

#include "Demo.h"
#include "MGameInstance.h"
#include "Developer/MSaveGame.h"
#include "Inventory/Inventory.h"
#include "Net/UnrealNetwork.h"

AMPlayerState::AMPlayerState()
{
	UserID = 0;
	
	InventoryModule = NewObject<UInventory>();

	bReplicates = true;
}

int32 AMPlayerState::GetRoleNum() const
{
	return UserData.RoleData.Num();
}

void AMPlayerState::Login(const FOnRpcResult& InCallback)
{
	if (GetLocalRole() != ROLE_AutonomousProxy && !HasAuthority())
		return;

	RpcManager.Add(TEXT("Login"), InCallback);
	
	//OnLoginResult = InCallback;
	if (const UMGameInstance* GameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		LoginReq(GameInstance->UserID, GameInstance->UserName);
	}
}

void AMPlayerState::LoginReq_Implementation(const uint64 ID, const FString& Name)
{
	// GetData from SaveGame
	// 如果没有用户就为其注册一个，并返回数据
	const UMGameInstance* GameInstance = Cast<UMGameInstance>(GetGameInstance());
	if (GameInstance && GameInstance->SaveGame)
	{
		UMSaveGame* SaveGame = GameInstance->SaveGame;
		if (const FMUserData* FoundData = SaveGame->FindUserDataRef(ID))
		{
			LoginAck(*FoundData);
		}
		else
		{
			// Create User
			FMUserData NewData;
			NewData.UserID = ID;
			NewData.UserName = Name;
			NewData.CreateDate = FDateTime::Now().GetTicks();

			UserID = ID;
			UserName = Name;
			
			if (SaveGame->CreateUser(NewData))
				LoginAck(NewData);
			else
			{
				LoginAck(FMUserData());
			}
		}
	}

	UE_LOG(LogProjectM, Log, TEXT("Server: User: %s enter the game"), *Name);
}

void AMPlayerState::LoginAck_Implementation(const FMUserData& InData)
{
	UserData = InData;
	
	ELoginCode Code = ELoginCode::Ok;
	if (UserData.UserID == 0)
		Code = ELoginCode::Unknown;

	if (const FOnRpcResult* Callback = RpcManager.Find(TEXT("Login")))
	{
		bool bOk = InData.UserID != 0;
		if (bOk)
		{
			UserData = InData;
		}
		
		bOk = Callback->ExecuteIfBound(bOk);
	}

	//OnLoginResult(Code);
	//OnLoginResult.Reset();
}

void AMPlayerState::K2_CreateRole(const FCreateRoleParams& InParam, const FOnRpcResult& InCallback)
{
	RpcManager.Add(TEXT("CreateRole"), InCallback);
	CreateRoleReq(UserData.UserID, InParam);
}

void AMPlayerState::K2_ChangeRole(const FString& InName, const FOnRpcResult& InCallback)
{
	RpcManager.Add(TEXT("ChangeRole"), InCallback);
	ChangeRole(GetUserID(), InName);
}

void AMPlayerState::ChangeRole_Implementation(const int32 InID, const FString& InName)
{
	
}

void AMPlayerState::ChangeRoleResult_Implementation(const bool bOk)
{
	if (const FOnRpcResult* Callback = RpcManager.Find(TEXT("ChangeRole")))
	{
		Callback->ExecuteIfBound(bOk);
	}
}

void AMPlayerState::CreateRoleReq_Implementation(const int32 InID, const FCreateRoleParams& InParam)
{
	// Todo 创建角色存档
	const UMGameInstance* GameInstance = Cast<UMGameInstance>(GetGameInstance());
	if (GameInstance && GameInstance->SaveGame)
	{
		UMSaveGame* SaveGame = GameInstance->SaveGame;
		if (SaveGame->CreateRole(InID, InParam))
			CreateRoleAck(SaveGame->FindUserData(InID));// 返回玩家存档
		else
		{
			CreateRoleAck(FMUserData());
		}

		return;
	}

	CreateRoleAck(FMUserData());
}

void AMPlayerState::CreateRoleAck_Implementation(const FMUserData& InData)
{
	
	if (const FOnRpcResult* Callback = RpcManager.Find(TEXT("CreateRole")))
	{
		bool bOk = InData.UserID != 0;
		if (bOk)
		{
			UserData = InData;
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
	SharedParams.Condition = COND_InitialOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(AMPlayerState, UserID, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AMPlayerState, UserName, SharedParams);
}
