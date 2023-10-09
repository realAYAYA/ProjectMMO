#include "MGameClientSubsystem.h"

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
	CreateSocket(ServerURL, ServerProtocol);

	OnConnectedServer = Callback;
}

void UMGameClientSubsystem::CreateSocket(const FString& ServerURL, const FString& ServerProtocol)
{
	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);
	Socket->OnConnected().AddUObject(this, &UMGameClientSubsystem::OnConnected);
	Socket->OnClosed().AddUObject(this, &UMGameClientSubsystem::OnClosed);
	Socket->OnConnectionError().AddUObject(this, &UMGameClientSubsystem::OnConnectionError);
	Socket->OnMessage().AddUObject(this, &UMGameClientSubsystem::OnMessage);
	Socket->OnRawMessage().AddUObject(this, &UMGameClientSubsystem::OnRawMessage);
	Socket->OnBinaryMessage().AddUObject(this, &UMGameClientSubsystem::OnBinaryMessage);
	Socket->OnMessageSent().AddUObject(this, &UMGameClientSubsystem::OnMessageSent);
	
	Socket->Connect();
}

void UMGameClientSubsystem::CloseSocket(const FOnDisConnectServer& Callback)
{
	OnDisConnectedServer = Callback;
	Socket->Close();
}

void UMGameClientSubsystem::Send(const TArray<uint8>& Data) const
{
	Socket->Send(Data.GetData(), Data.Num(), true);
}

void UMGameClientSubsystem::OnConnected() const
{
	OnConnectedServer.ExecuteIfBound(true);
}

void UMGameClientSubsystem::OnConnectionError(const FString& Error) const
{
	OnErrorCallback.Broadcast(Error);
}

void UMGameClientSubsystem::OnClosed(const int32 StatusCode, const FString& Reason, const bool bWasClean)
{
	OnDisConnectedServer.ExecuteIfBound();
	
	Socket = nullptr;
}

void UMGameClientSubsystem::OnMessage(const FString& Message)
{
}

void UMGameClientSubsystem::OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
	
}

void UMGameClientSubsystem::OnBinaryMessage(const void* Data, SIZE_T Size, bool bIsLastFragment)
{
}

void UMGameClientSubsystem::OnMessageSent(const FString& Message)
{
}
