// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerController.h"

#include "MBlueprintLibrary.h"
#include "MPlayerState.h"

AMPlayerState* AMPlayerController::GetMPlayerState() const
{
	return Cast<AMPlayerState>(PlayerState);
}

void AMPlayerController::K2_Login(const FString& Account, const FOnLoginResult& Callback)
{
	if (!IsValid(Callback.GetUObject()))
		return;

	const UGameRpcClient* RPC = UMBlueprintLibrary::GetClientRpcStub(GetWorld());
	AMPlayerState* PS = GetMPlayerState();
	if (!RPC || !PS)
	{
		Callback.Execute(ELoginGameRetCode::UnKnow, TArray<FPreviewRoleData>());
		return;
	}
	
	FLoginGameReq Req;
	Req.Account = Account;
	Req.ClientVersion = TEXT("");
	RPC->LoginGame(Req, [this, PS, Callback](const ERpcErrorCode ErrorCode, const FLoginGameAck& Ack)
	{
		if (ErrorCode != ERpcErrorCode::Ok)
		{
			Callback.Execute(ELoginGameRetCode::UnKnow, TArray<FPreviewRoleData>());
			return;
		}

		if (Ack.Ret == ELoginGameRetCode::Ok)
		{
			PS->SetPlayerID(Ack.PlayerID);
		}

		Callback.Execute(Ack.Ret, Ack.RolePreviewData);
	});
}

void AMPlayerController::K2_ReqMyRoleData(const FString& RoleName, const FOnRpcResult& Callback)
{
	if (!IsValid(Callback.GetUObject()))
		return;
	
	const AMPlayerState* PS = GetMPlayerState();
	if (!PS)
	{
		Callback.Execute(EOpErrorCode::BadMemory);
		return;
	}

	if (PS->GetPlayerID() == 0)
	{
		Callback.Execute(EOpErrorCode::BadParams);
		return;
	}
	
	if (RequestPendingData.Find(TEXT("GetMyRoleData")))
	{
		Callback.Execute(EOpErrorCode::Waiting);
		return;
	}

	RequestPendingData.Add(TEXT("GetMyRoleData"), Callback);
	GetMyRoleDataReq(RoleName);
}

void AMPlayerController::GetMyRoleDataReq_Implementation(const FString& RoleName)
{
	const UGameRpcClient* RPC = UMBlueprintLibrary::GetClientRpcStub(GetWorld());
	AMPlayerState* PS = GetMPlayerState();
	if (!RPC || !PS)
	{
		GetMyRoleDataAck(FRoleData(), EOpErrorCode::BadServer);
		return;
	}
	
	FPullRoleDataReq Req;
	Req.PlayerID = GetMPlayerState()->GetPlayerID();
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
	
	if (const FOnRpcResult* Callback = RequestPendingData.Find(TEXT("GetMyRoleData")))
	{
		if (IsValid(Callback->GetUObject()))
			Callback->Execute(Error);
		
		RequestPendingData.Remove(TEXT("GetMyRoleData"));
	}
}

void AMPlayerController::ShowNotice_Implementation(const FString& InMessage)
{
	OnNotice.Broadcast(InMessage);
}

