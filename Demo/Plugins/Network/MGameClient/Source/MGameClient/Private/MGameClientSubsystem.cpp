#include "MGameClientSubsystem.h"

#include "GameRpcClient.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"

bool UMGameClientSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UMGameClientSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UMGameClientSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UMGameClientSubsystem::Tick(float DeltaTime)
{
	
}

bool UMGameClientSubsystem::IsTickable() const
{
	return FTickableGameObject::IsTickable();
}

TStatId UMGameClientSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMGameClientSubsystem, STATGROUP_Tickables);
}

void UMGameClientSubsystem::K2_CreateSocket(const FString& ServerURL, const FString& ServerProtocol, const FOnConnectServer& Callback)
{
	OnConnectedServer = Callback;
	CreateSocket(ServerURL, ServerProtocol);
}

void UMGameClientSubsystem::CreateSocket(const FString& ServerURL, const FString& ServerProtocol)
{
	Connection = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);
	Connection->OnConnected().AddUObject(this, &UMGameClientSubsystem::OnConnected);
	Connection->OnClosed().AddUObject(this, &UMGameClientSubsystem::OnClosed);
	Connection->OnConnectionError().AddUObject(this, &UMGameClientSubsystem::OnConnectionError);
	//Connection->OnMessage().AddUObject(this, &UMGameClientSubsystem::OnMessage);
	Connection->OnRawMessage().AddUObject(this, &UMGameClientSubsystem::OnRawMessage);
	Connection->OnBinaryMessage().AddUObject(this, &UMGameClientSubsystem::OnBinaryMessage);
	//Connection->OnMessageSent().AddUObject(this, &UMGameClientSubsystem::OnMessageSent);
	
	Connection->Connect();
}

void UMGameClientSubsystem::CloseSocket(const FOnDisConnectServer& Callback)
{
	OnDisConnectedServer = Callback;
	Connection->Close();
}

void UMGameClientSubsystem::OnConnected()
{
	GameRpcClient->Setup(&RpcManager, Connection);
	
	OnConnectedServer.ExecuteIfBound(true);
}

void UMGameClientSubsystem::OnConnectionError(const FString& Error) const
{
	OnErrorCallback.Broadcast(Error);
}

void UMGameClientSubsystem::OnClosed(const int32 StatusCode, const FString& Reason, const bool bWasClean)
{
	GameRpcClient->Setup(nullptr, nullptr);
	Connection = nullptr;
	
	OnDisConnectedServer.ExecuteIfBound();
}

void UMGameClientSubsystem::OnRawMessage(const void* InData, SIZE_T Size, SIZE_T BytesRemaining)
{
	TArray<uint8> Data;
	Data.Reserve(Size);
	memcpy(Data.GetData(), InData, Size);

	FMRpcMessage RpcMessage;
	RpcMessage.ParseFromArray(Data);
	RpcManager.OnRpcMessage(Connection, RpcMessage);
}

void UMGameClientSubsystem::OnBinaryMessage(const void* InData, SIZE_T Size, bool bIsLastFragment)
{
	TArray<uint8> Data;
	Data.Reserve(Size);
	memcpy(Data.GetData(), InData, Size);

	FMRpcMessage RpcMessage;
	RpcMessage.ParseFromArray(Data);
	RpcManager.OnRpcMessage(Connection, RpcMessage);
}

