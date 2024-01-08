// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerController.h"

#include "Demo.h"
#include "MBlueprintLibrary.h"
#include "MGameModeBase.h"
#include "MPlayerState.h"
#include "Characters/MCharacter.h"
#include "Characters/MPlayerCharacter.h"

AMPlayerState* AMPlayerController::GetMPlayerState() const
{
	return Cast<AMPlayerState>(PlayerState);
}

void AMPlayerController::K2_ReqMyRoleData(const FOnRpcResult& Callback)
{
	const uint64 PlayerID = UMBlueprintLibrary::GetPlayerID(GetWorld());
	
	const AMPlayerState* PS = GetMPlayerState();
	if (!PS)
	{
		Callback.Execute(EOpErrorCode::BadMemory);
		return;
	}

	if (PlayerID == 0)
	{
		Callback.Execute(EOpErrorCode::BadParams);
		return;
	}
	
	if (RpcManager.FindRpcCallback(TEXT("GetMyRoleData")))
	{
		Callback.Execute(EOpErrorCode::Waiting);
		return;
	}

	RpcManager.AddRpcCallback(TEXT("GetMyRoleData"), Callback);
	GetMyRoleDataReq(PlayerID);
}

void AMPlayerController::K2_RequestPawn(const FTransform& SpawnTransform)
{
	if (GetMCharacterPawn())
		return;

	RequestPawn(SpawnTransform);
}


AMPlayerCharacter* AMPlayerController::GetMCharacterPawn() const
{
	return Cast<AMPlayerCharacter>(GetPawn());
}

void AMPlayerController::RequestPawn_Implementation(const FTransform& SpawnTransform)
{
	const AMGameMode* GameMode = Cast<AMGameMode>(GetWorld()->GetAuthGameMode());
	UClass* PawnClass = GameMode->MPlayerPawnClass;
	if (GameMode && PawnClass)
	{
		// Todo Spawn PlayerCharacter and init it
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = GetInstigator();
		SpawnInfo.ObjectFlags |= RF_Transient;
		
		APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo);
		if (AMPlayerCharacter* MCharacter = Cast<AMPlayerCharacter>(ResultPawn))
		{
			this->Possess(MCharacter);
			//MCharacter->LoadData();
		}
		else
		{
			if (ResultPawn)
				ResultPawn->Destroyed();
		}
	}
}

void AMPlayerController::GetMyRoleDataReq_Implementation(const uint64 InPlayerID)
{
	UE_LOG(LogProjectM, Warning, TEXT("Player(ID: %llu) Requesting RoleData..."), InPlayerID);
	
	if (InPlayerID == 0)
	{
		GetMyRoleDataAck(FRoleData(), EOpErrorCode::BadParams);
		return;
	}
	
	const UGameRpcClient* RPC = UMBlueprintLibrary::GetClientRpcStub(GetWorld());
	AMPlayerState* PS = GetMPlayerState();
	if (!RPC || !PS)
	{
		GetMyRoleDataAck(FRoleData(), EOpErrorCode::BadServer);
		return;
	}

	PS->SetPlayerID(InPlayerID);
	
	FPullRoleDataReq Req;
	Req.PlayerID = InPlayerID;
	RPC->PullRoleData(Req, [this, PS](const ERpcErrorCode ErrorCode, const FPullRoleDataAck& Ack)
	{
		if (ErrorCode != ERpcErrorCode::Ok || !Ack.bOk)
		{
			GetMyRoleDataAck(FRoleData(), EOpErrorCode::BadServer);
			return;
		}
		
		PS->LoadData(Ack.RoleData);
		
		GetMyRoleDataAck(Ack.RoleData, EOpErrorCode::Ok);
	});
}

void AMPlayerController::GetMyRoleDataAck_Implementation(const FRoleData& InData, EOpErrorCode Error)
{
	if (Error == EOpErrorCode::Ok && InData.ID != 0)
	{
		if (AMPlayerState* PS = GetMPlayerState())
		{
			PS->LoadData(InData);
		}
		else
		{
			Error = EOpErrorCode::BadMemory;
		}
	}
	
	if (const FOnRpcResult* Callback = RpcManager.FindRpcCallback(TEXT("GetMyRoleData")))
	{
		Callback->Execute(Error);
		RpcManager.RemoveRpcCallback(TEXT("GetMyRoleData"));
	}
}

void AMPlayerController::ShowNotice_Implementation(const FString& InMessage)
{
	OnStringNoticeFromDs.Broadcast(InMessage);
}
