#include "MGameServerSubsystem.h"
#include "MGameServerPrivate.h"

#include "IWebSocketNetworkingModule.h"

bool UMGameServerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UMGameServerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UMGameServerSubsystem::Deinitialize()
{
	StopWebSocketServer();
	
	Super::Deinitialize();
}

void UMGameServerSubsystem::Tick(float DeltaTime)
{
	if (IsServerRunning())
	{
		MWebSocketServer->Tick();
	}
}

bool UMGameServerSubsystem::IsTickable() const
{
	return FTickableGameObject::IsTickable();
}

TStatId UMGameServerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMWebSocketServerSubsystem, STATGROUP_Tickables);
}

bool UMGameServerSubsystem::StartWebSocketServer(const int32 ServerPort)
{
	FWebSocketClientConnectedCallBack CallBack;
	CallBack.BindUObject(this, &UMGameServerSubsystem::OnClientConnected);

	if (MWebSocketServer && MWebSocketServer.IsValid())
	{
		UE_LOG(LogMGameServer, Display, TEXT("%s: WebSocketServer is running."), *FString(__FUNCTION__));
		return false;
	}
	
	MWebSocketServer = FModuleManager::Get().LoadModuleChecked<IWebSocketNetworkingModule>(TEXT("WebSocketNetworking")).CreateServer();
	if (!MWebSocketServer || !MWebSocketServer->Init(ServerPort, CallBack))
	{
		UE_LOG(LogMGameServer, Display, TEXT("%s: WebSocketServer Init Failed."), *FString(__FUNCTION__));
		MWebSocketServer.Reset();
		return false;
	}

	UE_LOG(LogMGameServer, Display, TEXT("%s: WebSocketServer Init Succed."), *FString(__FUNCTION__));
	
	return true;
}

void UMGameServerSubsystem::StopWebSocketServer()
{
	UE_LOG(LogMGameServer, Display, TEXT("%s"), *FString(__FUNCTION__));

	if (IsServerRunning())
	{
		MWebSocketServer.Reset();
	}
}

void UMGameServerSubsystem::SendToAll(const FString& InMessage)
{
	const FTCHARToUTF8 UTF8Str(*InMessage);
	const int32 UTF8StrLen = UTF8Str.Length();
	TArray<uint8> UTF8Array;
	memcpy(UTF8Array.GetData(), UTF8Str.Get(), UTF8StrLen);
	
	SendToAll(UTF8Array);
}

void UMGameServerSubsystem::SendToAll(const TArray<uint8>& InData)
{
	for (const auto& Connection : Connections)
	{
		Connection.WebSocket->Send(InData.GetData(), InData.Num(), false);
	}
}

void UMGameServerSubsystem::SendTo(const FGuid InID, const TArray<uint8>& InData)
{
	if (const FMWebSocketConnection* Connection = Connections.FindByPredicate([&InID](const FMWebSocketConnection& Elem)
	{
		return Elem.ID == InID;
	}))
	{
		Connection->WebSocket->Send(InData.GetData(), InData.Num(), false);
	}
}

bool UMGameServerSubsystem::CheckConnectionValid(const FGuid InID)
{
	if (!IsServerRunning())
		return false;

	const FMWebSocketConnection* Connection = Connections.FindByPredicate([&InID](const FMWebSocketConnection& Elem)
	{
		return Elem.ID == InID;
	});

	if (!Connection->WebSocket)
		return false;
	
	return true;
}

void UMGameServerSubsystem::OnClientConnected(INetworkingWebSocket* InWebSocket)
{
	UE_LOG(LogMGameServer, Display, TEXT("%s"), *FString(__FUNCTION__));

	if (!InWebSocket)
		return;

	FMWebSocketConnection WebSocketConnection = FMWebSocketConnection { InWebSocket };
	
	FWebSocketInfoCallBack ConnectedCallBack;
	ConnectedCallBack.BindUObject(this, &UMGameServerSubsystem::OnConnected, WebSocketConnection.ID);
	InWebSocket->SetConnectedCallBack(ConnectedCallBack);

	FWebSocketInfoCallBack ErrorCallBack;
	ErrorCallBack.BindUObject(this, &UMGameServerSubsystem::OnError, WebSocketConnection.ID);
	InWebSocket->SetErrorCallBack(ErrorCallBack);
	
	FWebSocketPacketReceivedCallBack ReceivedCallBack;
	ReceivedCallBack.BindUObject(this, &UMGameServerSubsystem::OnReceive, WebSocketConnection.ID);
	InWebSocket->SetReceiveCallBack(ReceivedCallBack);

	FWebSocketInfoCallBack ClosedCallBack;
	ClosedCallBack.BindUObject(this, &UMGameServerSubsystem::OnClosed, WebSocketConnection.ID);
	InWebSocket->SetSocketClosedCallBack(ClosedCallBack);

	Connections.Add(MoveTemp(WebSocketConnection));
}

bool UMGameServerSubsystem::IsServerRunning() const
{
	return MWebSocketServer && MWebSocketServer.IsValid();
}

void UMGameServerSubsystem::OnConnected(const FGuid InID) const
{
	UE_LOG(LogMGameServer, Display, TEXT("User: %s - %s"), *InID.ToString(), *FString(__FUNCTION__));
}

void UMGameServerSubsystem::OnReceive(void* InData, const int32 DataSize, const FGuid InID)
{
	const TArrayView<uint8> DataArrayView = MakeArrayView(static_cast<uint8*>(InData), DataSize);

	const std::string CStr(reinterpret_cast<const char*>(DataArrayView.GetData(), DataArrayView.Num()));
	const FString Str = UTF8_TO_TCHAR(CStr.c_str());

	ProcessAllClientInfo(InID, Str);
}

void UMGameServerSubsystem::OnError(const FGuid InID)
{
	UE_LOG(LogMGameServer, Warning, TEXT("User: %s - %s"), *InID.ToString(), *FString(__FUNCTION__));
}

void UMGameServerSubsystem::OnClosed(const FGuid InID)
{
	const int32 Index = Connections.IndexOfByPredicate([InID](const FMWebSocketConnection& Connection)
	{
		return Connection.ID == InID;
	});

	if (Index != INDEX_NONE)
	{
		WebSocketClientClosedCallBack.ExecuteIfBound(InID);
		Connections.RemoveAtSwap(Index);
	}

	UE_LOG(LogMGameServer, Display, TEXT("%s"), *FString(__FUNCTION__));
}

void UMGameServerSubsystem::ProcessAllClientInfo(const FGuid ClientID, const FString& Info)
{
	
}
