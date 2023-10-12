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
	
	const bool bOk = OnConnectedServer.ExecuteIfBound(true);
	if (!bOk)
		UE_LOG(LogMGameClient, Warning, TEXT("%s : InValid Callback"), *FString(__FUNCTION__));
}

void UMGameClientSubsystem::OnConnectionError(const FString& Error) const
{
	OnErrorCallback.Broadcast(Error);
}

void UMGameClientSubsystem::OnClosed(const int32 StatusCode, const FString& Reason, const bool bWasClean)
{
	GameRpcClient->Setup(nullptr, nullptr);
	Connection = nullptr;
	
	const bool bOk = OnDisConnectedServer.ExecuteIfBound();
	if (!bOk)
		UE_LOG(LogMGameClient, Warning, TEXT("%s : InValid Callback"), *FString(__FUNCTION__));
}

void UMGameClientSubsystem::OnRawMessage(const void* InData, SIZE_T Size, SIZE_T BytesRemaining)
{
	TArray<uint8> Data;
	Data.Reserve(Size);
	memcpy(Data.GetData(), InData, Size);

	FNetworkMessage RpcMessage;
	RpcMessage.ParseFromArray(Data);
	RpcManager.OnMessage(Connection, RpcMessage);
}

void UMGameClientSubsystem::OnBinaryMessage(const void* InData, SIZE_T Size, bool bIsLastFragment)
{
	TArray<uint8> Data;
	Data.Reserve(Size);
	memcpy(Data.GetData(), InData, Size);

	FNetworkMessage RpcMessage;
	RpcMessage.ParseFromArray(Data);
	RpcManager.OnMessage(Connection, RpcMessage);
}

