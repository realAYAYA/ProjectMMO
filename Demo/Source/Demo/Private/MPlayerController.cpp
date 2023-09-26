// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerController.h"

#include "Demo.h"
#include "MGameInstance.h"
#include "MPlayerState.h"
#include "Characters/MCharacter.h"
#include "Developer/MSaveGame.h"
#include "Net/UnrealNetwork.h"

AMPlayerController::AMPlayerController(const FObjectInitializer& ObjectInitializer)
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
		if (Code == ELoginCode::Ok)
			OnFinishedLogin.Broadcast(true);

		OnFinishedLogin.Broadcast(false);
	});
}

void AMPlayerController::K2_CreateRole(const FCreateRoleParams& InParam, const FOnRpcResult& InCallback)
{
	const AMPlayerState* PS = GetMPlayerState();
	if (PS && PS->IsOnline())
	{
		RpcManager.Add(TEXT("CreateRole"), InCallback);
		CreateRoleReq(PS->GetUserID(), InParam);
	}
}

void AMPlayerController::K2_ChooseRole(const FString& InName, const FOnRpcResult& InCallback)
{
	const AMPlayerState* PS = GetMPlayerState();
	if (PS && PS->IsOnline())
	{
		RpcManager.Add(TEXT("ChooseRole"), InCallback);
		ChooseRoleReq(PS->GetUserID(), InName);
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

void AMPlayerController::LoginReq_Implementation(const uint64 ID, const FString& Name)
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
			
			if (SaveGame->CreateUser(NewData))
				LoginAck(NewData);
			else
			{
				LoginAck(FMUserData());
			}
		}
	}

	UserID = ID;
	UserName = Name;

	// 为角色改名
	if (AMCharacter* MCharacter = Cast<AMCharacter>(GetPawn()))
	{
		MCharacter->RoleName = Name;
	}

	UE_LOG(LogProjectM, Log, TEXT("Server: User: %s enter the game"), *Name);
}

void AMPlayerController::LoginAck_Implementation(const FMUserData& InData)
{
	AMPlayerState* PS = GetMPlayerState();
	if (!PS || InData.UserID <= 0)
	{
		OnLoginCallback(ELoginCode::Unknown);
		return;
	}
	
	PS->SetUserData(InData);

	OnLoginCallback(ELoginCode::Ok);
}

void AMPlayerController::CreateRoleReq_Implementation(const uint64 InID, const FCreateRoleParams& InParam)
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
		
		Callback->ExecuteIfBound(bOk);
	}
}

void AMPlayerController::ChooseRoleReq_Implementation(const uint64 InID, const FString& InName)
{
	
}

void AMPlayerController::ChooseRoleAck_Implementation(const bool bOk)
{
	if (const FOnRpcResult* Callback = RpcManager.Find(TEXT("ChangeRole")))
	{
		Callback->ExecuteIfBound(bOk);
	}
}

void AMPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.Condition = COND_InitialOnly;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(AMPlayerController, UserID, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AMPlayerController, UserName, SharedParams);
}