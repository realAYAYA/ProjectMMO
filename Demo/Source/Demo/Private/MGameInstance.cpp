// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameInstance.h"

#include "AbilitySystemGlobals.h"
#include "Demo.h"
#include "MGameClientSubsystem.h"
#include "../../../Plugins/Network/MRpc/Source/MRpc/Public/GameRpcClient.h"
#include "Developer/MSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UMGameInstance::Init()
{
	Super::Init();

	UAbilitySystemGlobals::Get().InitGlobalData();
	
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

	LoginToWebSocketServer();
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

void UMGameInstance::LoginToWebSocketServer()
{
	if (!IsRunningDedicatedServer())
	{
		return;
	}
	
	if (UMGameClientSubsystem* Client = GetSubsystem<UMGameClientSubsystem>())
	{
		Client->GetOnConnectedCallback().BindUFunction(this, TEXT("GetReady"));

		Client->CreateSocket(TEXT(""), TEXT("10086"));
	}
}

void UMGameInstance::GetReady(bool bOk)
{
	// Todo 如果是DS服务器，发起请求连接至WebSocket服务器
	if (const UGameRpcClient* Stub = GetClientRpcStub())
	{
		//GetToken();
		//
		FLoginAsDSReq Req;
		Stub->LoginAsDS(Req, [this](ERpcErrorCode Code, const FLoginAsDSAck& Ack)
		{
			if (Code == ERpcErrorCode::Ok && Ack.bOk)
			{
				// 拿到游戏存档初始化世界
				bReady = true;
			}
		});
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

