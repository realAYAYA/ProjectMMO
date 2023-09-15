#include "MWebSocketServerSubsystem.h"
#include "MWebSocketServerPrivate.h"

#include "IWebSocketNetworkingModule.h"
#include "IWebSocketServer.h"

bool UMWebSocketServerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UMWebSocketServerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UMWebSocketServerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UMWebSocketServerSubsystem::Tick(float DeltaTime)
{
}

bool UMWebSocketServerSubsystem::IsTickable() const
{
	return FTickableGameObject::IsTickable();
}

TStatId UMWebSocketServerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMWebSocketServerSubsystem, STATGROUP_Tickables);
}

bool UMWebSocketServerSubsystem::StartWebSocketServer(const int32 ServerPort)
{
	FWebSocketClientConnectedCallBack CallBack;
	CallBack.BindUObject(this, &UMWebSocketServerSubsystem::OnClientConnected);

	if (MWebSocketServer && MWebSocketServer.IsValid())
	{
		UE_LOG(LogMWebSocketServer, Display, TEXT("%s: WebSocketServer is running."), *FString(__FUNCTION__));
		return false;
	}
	
	MWebSocketServer = FModuleManager::Get().LoadModuleChecked<IWebSocketNetworkingModule>(TEXT("WebSocketNetworking")).CreateServer();
	if (!MWebSocketServer || !MWebSocketServer->Init(ServerPort, CallBack))
	{
		UE_LOG(LogMWebSocketServer, Display, TEXT("%s: WebSocketServer Init Failed."), *FString(__FUNCTION__));
		MWebSocketServer.Reset();
		return false;
	}

	UE_LOG(LogMWebSocketServer, Display, TEXT("%s: WebSocketServer Init Succed."), *FString(__FUNCTION__));
	
	return true;
}

void UMWebSocketServerSubsystem::StopWebSocketServer(const int32 ServerPort)
{
	UE_LOG(LogMWebSocketServer, Display, TEXT("%s"), *FString(__FUNCTION__));

	if (IsServerRunning())
	{
		MWebSocketServer.Reset();
	}
}

void UMWebSocketServerSubsystem::SendToAll(const int32 InMessage)
{
}

void UMWebSocketServerSubsystem::SendTo(const FGuid InID, const int32 InMessage)
{
}

bool UMWebSocketServerSubsystem::CheckConnectionValid(const FGuid InID)
{
	return false;
}

void UMWebSocketServerSubsystem::OnClientConnected(INetworkingWebSocket* InWebSocket)
{
	UE_LOG(LogMWebSocketServer, Display, TEXT("%s"), *FString(__FUNCTION__));

	if (!InWebSocket)
		return;

	const FMWebSocketConnection WebSocketConnection = FMWebSocketConnection { InWebSocket };
	FWebSocketInfoCallBack ConnectedCallBack;
	ConnectedCallBack.BindUObject(this, &UMWebSocketServerSubsystem::OnConnected, WebSocketConnection.ID);
	InWebSocket->SetConnectedCallBack(ConnectedCallBack);
	
}

bool UMWebSocketServerSubsystem::IsServerRunning() const
{
	return MWebSocketServer && MWebSocketServer.IsValid();
}

void UMWebSocketServerSubsystem::OnConnected(const FGuid InID)
{
}

void UMWebSocketServerSubsystem::OnReceive(const FGuid InID, void* Data)
{
}

void UMWebSocketServerSubsystem::OnError(const FGuid InID)
{
}

void UMWebSocketServerSubsystem::OnClosed(const FGuid InID)
{
}

void UMWebSocketServerSubsystem::SendToPointedClient(const FGuid& InClientID, const TArray<uint8>& InUTF8Payload)
{
}

void UMWebSocketServerSubsystem::ProcessAllClientInfo(const FGuid ClientID, const int32 Info)
{
}
