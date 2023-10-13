#include "MGameServerSubsystem.h"
#include "MGameServerPrivate.h"

#include "IWebSocketNetworkingModule.h"

bool UMGameServerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;//IsRunningDedicatedServer();
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
		WebSocketServer->Tick();
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

bool UMGameServerSubsystem::K2_StartWebSocketServer(const int32 ServerPort)
{
	return StartWebSocketServer(ServerPort);
}

void UMGameServerSubsystem::K2_SendToAll(const FString& InMessage)
{
	const FTCHARToUTF8 UTF8(*InMessage);
	const int32 Len = UTF8.Length();
	TArray<uint8> Data;
	Data.SetNum(Len);
	memcpy(Data.GetData(), UTF8.Get(), Len);

	for (const auto& Elem : Connections)
	{
		Elem.Value->Send(Data);
	}
}

bool UMGameServerSubsystem::StartWebSocketServer(const int32 ServerPort)
{
	FWebSocketClientConnectedCallBack CallBack;
	CallBack.BindUObject(this, &UMGameServerSubsystem::OnClientConnected);

	if (WebSocketServer && WebSocketServer.IsValid())
	{
		UE_LOG(LogMGameServer, Display, TEXT("%s: WebSocketServer is running."), *FString(__FUNCTION__));
		return false;
	}
	
	WebSocketServer = FModuleManager::Get().LoadModuleChecked<IWebSocketNetworkingModule>(TEXT("WebSocketNetworking")).CreateServer();
	if (!WebSocketServer || !WebSocketServer->Init(ServerPort, CallBack))
	{
		UE_LOG(LogMGameServer, Display, TEXT("%s: WebSocketServer Init Failed."), *FString(__FUNCTION__));
		WebSocketServer.Reset();
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
		WebSocketServer.Reset();
	}
}

void UMGameServerSubsystem::SendToAll(const FGameMessage& InMessage)
{
	FNetworkMessage RpcMessage;
	RpcMessage.RpcErrorCode = ERpcErrorCode::Ok;
	RpcMessage.RpcMessageOp = ERpcMessageOp::Notify;
	
	InMessage.SerializeToArray(RpcMessage.SetBody());

	TArray<uint8> Data;
	RpcMessage.SerializeToArray(Data);

	SendToAll(Data);
}

void UMGameServerSubsystem::SendToAll(const TArray<uint8>& InData)
{
	for (const auto& Elem : Connections)
	{
		const FGameSessionPtr& Connection = Elem.Value;
		if (Connection->WebSocket)
			Connection->Send(InData);
	}
}

bool UMGameServerSubsystem::CheckConnectionValid(const FGuid InID)
{
	if (!IsServerRunning())
		return false;

	const FGameSessionPtr* Connection = Connections.Find(InID);
	if (Connection && !(*Connection)->WebSocket)
		return false;
	
	return true;
}

void UMGameServerSubsystem::OnClientConnected(INetworkingWebSocket* InWebSocket)
{
	if (!InWebSocket)
		return;

	FGameSessionPtr Conn = MakeShared<FGameSession>(InWebSocket);
	
	FWebSocketInfoCallBack ConnectedCallBack;
	ConnectedCallBack.BindUObject(this, &UMGameServerSubsystem::OnConnected, Conn->ID);
	InWebSocket->SetConnectedCallBack(ConnectedCallBack);

	FWebSocketPacketReceivedCallBack ReceivedCallBack;
	ReceivedCallBack.BindUObject(this, &UMGameServerSubsystem::OnReceive, Conn->ID);
	InWebSocket->SetReceiveCallBack(ReceivedCallBack);

	FWebSocketInfoCallBack ErrorCallBack;
	ErrorCallBack.BindUObject(this, &UMGameServerSubsystem::OnError, Conn->ID);
	InWebSocket->SetErrorCallBack(ErrorCallBack);

	FWebSocketInfoCallBack ClosedCallBack;
	ClosedCallBack.BindUObject(this, &UMGameServerSubsystem::OnClosed, Conn->ID);
	InWebSocket->SetSocketClosedCallBack(ClosedCallBack);

	Connections.Emplace(Conn->ID, Conn);

	UE_LOG(LogMGameServer, Display, TEXT("%s"), *FString(__FUNCTION__));
}

bool UMGameServerSubsystem::IsServerRunning() const
{
	return WebSocketServer && WebSocketServer.IsValid();
}

void UMGameServerSubsystem::OnConnected(const FGuid InID)
{
	// Todo ReConnected
	const FGameSessionPtr* Connection = Connections.Find(InID);
	if (Connection && (*Connection)->WebSocket)
	{
		(*Connection)->OnConnected(InID);
		UE_LOG(LogMGameServer, Warning, TEXT("User: %s - %s"), *(*Connection)->Account, *FString(__FUNCTION__));
	}
}

void UMGameServerSubsystem::OnReceive(void* InData, const int32 DataSize, const FGuid InID)
{
	const FGameSessionPtr* Connection = Connections.Find(InID);

	if (Connection && (*Connection)->WebSocket)
	{
		(*Connection)->OnReceive(InData, DataSize);
	}
}

void UMGameServerSubsystem::OnError(const FGuid InID)
{
	const FGameSessionPtr* Connection = Connections.Find(InID);
	if (Connection && (*Connection)->WebSocket)
	{
		UE_LOG(LogMGameServer, Warning, TEXT("User: %s - %s"), *(*Connection)->Account, *FString(__FUNCTION__));
	}
}

void UMGameServerSubsystem::OnClosed(const FGuid InID)
{
	const FGameSessionPtr* Connection = Connections.Find(InID);
	if (Connection && (*Connection)->WebSocket)
	{
		const bool bOk = WebSocketClientClosedCallBack.ExecuteIfBound(InID);
		if (!bOk)
			UE_LOG(LogMGameServer, Warning, TEXT("%s : InValid Callback"), *FString(__FUNCTION__));
		
		Connections.Remove(InID);
	}

	UE_LOG(LogMGameServer, Display, TEXT("%s"), *FString(__FUNCTION__));
}
