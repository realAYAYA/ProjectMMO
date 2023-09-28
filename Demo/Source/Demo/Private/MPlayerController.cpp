// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerController.h"

#include "Demo.h"
#include "DemoGameMode.h"
#include "MGameInstance.h"
#include "MPlayerState.h"
#include "Characters/MCharacter.h"
#include "Developer/MSaveGame.h"
#include "Net/UnrealNetwork.h"

AMPlayerController::AMPlayerController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

bool AMPlayerController::IsOnline() const
{
	const AMPlayerState* PS = GetMPlayerState();
	if (PS && PS->IsOnline())
	{
		return true;
	}

	return false;
}

AMPlayerState* AMPlayerController::GetMPlayerState() const
{
 	return Cast<AMPlayerState>(PlayerState);
}

FString AMPlayerController::K2_GetUserID() const
{
	if (const AMPlayerState* PS = GetMPlayerState())
	{
		return PS->UserID;
	}

	return TEXT("");
}

void AMPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
}

void AMPlayerController::K2_Login()
{
	Login([this](const ELoginCode Code)
	{
		OnFinishedLogin.Broadcast(Code);
	});
}

void AMPlayerController::K2_CreateRole(const FCreateRoleParams& InParam, const FOnRpcResult& InCallback)
{
	const AMPlayerState* PS = GetMPlayerState();
	if (PS && PS->IsOnline())
	{
		RpcManager.Add(TEXT("CreateRole"), InCallback);
		CreateRoleReq(PS->UserID, InParam);
	}
}

void AMPlayerController::K2_ChooseRole(const FString& InName, const FOnRpcResult& InCallback)
{
	const AMPlayerState* PS = GetMPlayerState();
	if (PS && PS->IsOnline())
	{
		RpcManager.Add(TEXT("ChooseRole"), InCallback);
		ChooseRoleReq(PS->UserID, InName);
	}
}

void AMPlayerController::Login(const FOnLoginResult& InCallback)
{
	OnLoginCallback = InCallback;
	
	if (const UMGameInstance* GameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		LoginReq(GameInstance->UserID, GameInstance->UserName);
	}
}

void AMPlayerController::LoginReq_Implementation(const FString& InID, const FString& Name)
{
	// 玩家在线
	if (const AMGameMode* GameMode = Cast<AMGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (GameMode->FindOnlinePlayerByID(InID))
		{
			LoginAck(ELoginCode::DuplicateLogin, FMUserData());
			return;
		}
	}

	// GetData from SaveGame
	// 如果没有用户就为其注册一个，并返回数据
	const UMGameInstance* GameInstance = Cast<UMGameInstance>(GetGameInstance());
	AMPlayerState* PS = GetMPlayerState();
	if (GameInstance && GameInstance->SaveGame && PS)
	{
		UMSaveGame* SaveGame = GameInstance->SaveGame;
		if (const FMUserData* FoundData = SaveGame->FindUserDataRef(InID))
		{
			PS->SetUserData(*FoundData);
			LoginAck(ELoginCode::Ok, *FoundData);
		}
		else
		{
			// Create User
			FMUserData NewData;
			NewData.UserID = InID;
			NewData.UserName = Name;
			NewData.CreateDate = FDateTime::Now().GetTicks();
			
			if (SaveGame->CreateUser(NewData))
			{
				PS->SetUserData(NewData);
				LoginAck(ELoginCode::Ok, NewData);
			}
			else
			{
				LoginAck(ELoginCode::Unknown, FMUserData());
			}
		}
	}

	// Todo 为角色改名，以后会删掉
	if (AMCharacter* MCharacter = Cast<AMCharacter>(GetPawn()))
	{
		MCharacter->RoleName = InID;
	}

	UE_LOG(LogProjectM, Log, TEXT("Server: User: %s enter the game"), *Name);
}

void AMPlayerController::LoginAck_Implementation(const ELoginCode Code, const FMUserData& InData)
{
	AMPlayerState* PS = GetMPlayerState();
	if (PS && Code == ELoginCode::Ok)
	{
		PS->SetUserData(InData);
	}
	
	OnLoginCallback(Code);
}

void AMPlayerController::CreateRoleReq_Implementation(const FString& InID, const FCreateRoleParams& InParam)
{
	// Todo 创建角色存档
	const UMGameInstance* GameInstance = Cast<UMGameInstance>(GetGameInstance());
	if (GameInstance && GameInstance->SaveGame)
	{
		UMSaveGame* SaveGame = GameInstance->SaveGame;
		if (SaveGame->CreateRole(InID, InParam))
			CreateRoleAck(SaveGame->GetRoleData(InID, InParam.RoleName));// 返回玩家存档
		else
		{
			CreateRoleAck(FRoleData());
		}

		return;
	}

	CreateRoleAck(FRoleData());
}

void AMPlayerController::CreateRoleAck_Implementation(const FRoleData& InData)
{
	if (const FOnRpcResult* Callback = RpcManager.Find(TEXT("CreateRole")))
	{
		const bool bOk = InData.CreateDate != 0;
		if (bOk)
		{
			// Todo AddToData
		}
		
		Callback->ExecuteIfBound(ERpcErrorCode::Ok);
	}
}

void AMPlayerController::ChooseRoleReq_Implementation(const FString& InID, const FString& InName)
{
	
}

void AMPlayerController::ChooseRoleAck_Implementation(const bool bOk)
{
	if (const FOnRpcResult* Callback = RpcManager.Find(TEXT("ChangeRole")))
	{
		Callback->ExecuteIfBound(ERpcErrorCode::Ok);
	}
}