#include "MGameClientSubsystem.h"

#include "GameRpcClient.h"
#include "IWebSocket.h"
#include "MGameClientPrivate.h"
#include "WebSocketsModule.h"

bool UMGameClientSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UMGameClientSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	GameRpcClient = NewObject<UGameRpcClient>(this);
}

void UMGameClientSubsystem::Deinitialize()
{
	Super::Deinitialize();

	if (Connection)
	{
		Connection->Close();
	}
}

void UMGameClientSubsystem::K2_CreateSocket(const FString& ServerURL, const FString& ServerProtocol, const FOnConnectServer& Callback)
{
	OnConnectedServer = Callback;
	
	if (Connection && IsValid(OnConnectedServer.GetUObject()))
	{
		OnConnectedServer.Execute(false);// 连接已存在
		OnConnectedServer.Clear();
		return;
	}
	
	CreateSocket(ServerURL, ServerProtocol);
}

void UMGameClientSubsystem::CreateSocket(const FString& ServerURL, const FString& ServerProtocol)
{
	Connection = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);
	if (!Connection && IsValid(OnConnectedServer.GetUObject()))
	{
		OnConnectedServer.Execute(false);
		OnConnectedServer.Clear();
		return;
	}
	
	Connection->OnConnected().AddUObject(this, &UMGameClientSubsystem::OnConnected);
	Connection->OnClosed().AddUObject(this, &UMGameClientSubsystem::OnClosed);
	Connection->OnConnectionError().AddUObject(this, &UMGameClientSubsystem::OnConnectionError);
	//Connection->OnMessage().AddUObject(this, &UMGameClientSubsystem::OnMessage);
	Connection->OnRawMessage().AddUObject(this, &UMGameClientSubsystem::OnRawMessage);
	//Connection->OnBinaryMessage().AddUObject(this, &UMGameClientSubsystem::OnBinaryMessage);
	//Connection->OnMessageSent().AddUObject(this, &UMGameClientSubsystem::OnMessageSent);
	
	Connection->Connect();
}

void UMGameClientSubsystem::CloseSocket(const FOnDisConnectServer& Callback)
{
	OnDisConnectedServer = Callback;
	Connection->Close();
	Connection.Reset();
}

void UMGameClientSubsystem::OnConnected()
{
	GameRpcClient->Setup(&RpcManager, Connection);
	
	if (!OnConnectedServer.ExecuteIfBound(true))
	{
		UE_LOG(LogMGameClient, Warning, TEXT("%s : InValid Callback"), *FString(__FUNCTION__));
	}

	OnConnectedServer.Clear();
}

void UMGameClientSubsystem::OnConnectionError(const FString& Error)
{
	if (IsValid(OnConnectedServer.GetUObject()))
	{
		OnConnectedServer.Execute(false);
		OnConnectedServer.Clear();
	}
	
	OnErrorCallback.Broadcast(Error);
	OnErrorCallback.Clear();
	
	Connection->Close();
	Connection.Reset();

	UE_LOG(LogMGameClient, Warning, TEXT("%s"), *FString(__FUNCTION__));
}

void UMGameClientSubsystem::OnClosed(const int32 StatusCode, const FString& Reason, const bool bWasClean)
{
	GameRpcClient->Setup(nullptr, nullptr);
	Connection.Reset();
	
	if (!OnDisConnectedServer.ExecuteIfBound())
		UE_LOG(LogMGameClient, Warning, TEXT("%s : InValid Callback"), *FString(__FUNCTION__));
}

void UMGameClientSubsystem::OnRawMessage(const void* InData, SIZE_T Size, SIZE_T BytesRemaining)
{
	TArray<uint8> Data;
	Data.Append((uint8*)InData, Size);

	FNetworkMessage RpcMessage;
	RpcMessage.ParseFromArray(Data);
	RpcManager.OnMessage(Connection, RpcMessage);
}

/*void UMGameClientSubsystem::OnBinaryMessage(const void* InData, SIZE_T Size, bool bIsLastFragment)
{
	TArray<uint8> Data;
	Data.Append((uint8*)InData, Size);

	FNetworkMessage RpcMessage;
	RpcMessage.ParseFromArray(Data);
	RpcManager.OnMessage(Connection, RpcMessage);
}*/

