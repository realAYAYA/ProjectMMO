// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameInstance.h"

#include "AbilitySystemGlobals.h"
#include "Demo.h"
#include "MGameClientSubsystem.h"
#include "GameRpcClient.h"
#include "Developer/MSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UMGameInstance::Init()
{
	Super::Init();

	UAbilitySystemGlobals::Get().InitGlobalData();

	LoadLocalSave();

	LoginServer();
}

void UMGameInstance::BeginDestroy()
{
	if (SaveGame)
	{
		if (!UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("MSaveGame"), 0))
		{
			UE_LOG(LogProjectM, Error, TEXT("SaveGame Failed."));
		}
	}
	
	Super::BeginDestroy();
}

UMGameInstance* UMGameInstance::GetMGameInstance(const UWorld* World)
{
	return World ? Cast<UMGameInstance>(World->GetGameInstance()) : nullptr;
}

void UMGameInstance::LoadLocalSave()
{
	if (IsRunningDedicatedServer())
		return;
	
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("MSaveGame"), 0))
	{
		SaveGame = NewObject<UMSaveGame>(this);
	}
	else
	{
		SaveGame = Cast<UMSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MSaveGame"), 0));
		if (!SaveGame)
			SaveGame = NewObject<UMSaveGame>(this);
	}
}

void UMGameInstance::LoginServer()
{
	if (!IsRunningDedicatedServer())
	{
		return;
	}
	
	if (UMGameClientSubsystem* Client = GetSubsystem<UMGameClientSubsystem>())
	{
		Client->GetOnConnectedCallback().BindUFunction(this, TEXT("GetReady"));

		Client->CreateSocket(TEXT("ws://127.0.0.1:10086"), TEXT(""));

		//Client->OnErrorCallback.AddDynamic(); //Todo 连接断开
		//Client->CloseSocket()
	}
}

void UMGameInstance::CloseConnection()
{
}

void UMGameInstance::OnDisConnected()
{
}

void UMGameInstance::GetReady(bool Success)
{
	if (!Success)
	{
		// 连接到根服务器不成功，重连多次还失败，则关闭DS
		UE_LOG(LogProjectM, Error, TEXT("DS Link root server failed!"));
		return;
	}
	
	// Todo 如果是DS服务器，发起请求连接至WebSocket服务器
	if (const UGameRpcClient* Stub = GetClientRpcStub())
	{
		//GetToken();
		//
		FLoginAsDSReq Req;
		Req.Token = 123456;
		Stub->LoginAsDS(Req, [this](ERpcErrorCode Code, const FLoginAsDSAck& Ack)
		{
			if (Code == ERpcErrorCode::Ok && Ack.bOk)
			{
				// 拿到游戏存档初始化世界
				bReady = true;
				UE_LOG(LogProjectM, Warning, TEXT("Level GetReady!"));
			}
			else if (Code == ERpcErrorCode::TimeOut)
			{
				GetReady(true);
				UE_LOG(LogProjectM, Warning, TEXT("Login Timeout, Try again!"));
			}
		});
	}
	else
	{
		UE_LOG(LogProjectM, Error, TEXT("Get Client Stub Failed!"));
	}
}

UGameRpcClient* UMGameInstance::GetClientRpcStub() const
{
	if (const UMGameClientSubsystem* Client = GetSubsystem<UMGameClientSubsystem>())
	{
		if (Client->IsConnected())
			return Client->GameRpcClient;
	}

	return nullptr;
}

void UMGameInstance::SetLoginInfo(const FString& InUserID, const FString& InUserName)
{
	// Todo 从Steam获取信息
	UserID = InUserID;
	UserName = InUserName;
}

void UMGameInstance::K2_Login(const FString& Account, const FOnLoginResult& Callback)
{
	if (!IsValid(Callback.GetUObject()))
		return;

	const UGameRpcClient* RPC = GetClientRpcStub();
	
	FLoginGameReq Req;
	Req.Account = Account;
	Req.ClientVersion = TEXT("");
	RPC->LoginGame(Req, [this, Callback](const ERpcErrorCode ErrorCode, const FLoginGameAck& Ack)
	{
		if (ErrorCode != ERpcErrorCode::Ok)
		{
			Callback.Execute(ELoginGameRetCode::UnKnow, TArray<FPreviewRoleData>());
			return;
		}

		PlayerID = Ack.PlayerID;

		Callback.Execute(Ack.Ret, Ack.RolePreviewData);
	});
}
