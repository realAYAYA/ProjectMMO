#include "MGameClientSubsystem.h"

#include "WebSocketsModule.h"

bool UMWebSocketClientSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UMWebSocketClientSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UMWebSocketClientSubsystem::Deinitialize()
{
	
	Super::Deinitialize();
}

void UMWebSocketClientSubsystem::Tick(float DeltaTime)
{
	
}

bool UMWebSocketClientSubsystem::IsTickable() const
{
	return FTickableGameObject::IsTickable();
}

TStatId UMWebSocketClientSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMWebSocketServerSubsystem, STATGROUP_Tickables);
}

void UMWebSocketClientSubsystem::CreateSocket()
{
	const FString ServerURL = TEXT("ws://127.0.0.1:9093");
	const FString ServerProtocol = TEXT("");

	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);
	Socket->OnConnected().AddUObject(this, &UMWebSocketClientSubsystem::OnConnected);
	Socket->OnClosed().AddUObject(this, &UMWebSocketClientSubsystem::OnClosed);
	Socket->OnConnectionError().AddUObject(this, &UMWebSocketClientSubsystem::OnConnectionError);
	Socket->OnMessage().AddUObject(this, &UMWebSocketClientSubsystem::OnMessage);
	Socket->OnMessageSent().AddUObject(this, &UMWebSocketClientSubsystem::OnMessageSent);

	Socket->Connect();
}

void UMWebSocketClientSubsystem::CloseSocket()
{
}

void UMWebSocketClientSubsystem::SendMessage(const FString& InMessage)
{
	
}

void UMWebSocketClientSubsystem::OnConnected() const
{
	
}

void UMWebSocketClientSubsystem::OnConnectionError(const FString& Error)
{
}

void UMWebSocketClientSubsystem::OnClosed(const int32 StatusCode, const FString& Reason, const bool bWasClean)
{
}

void UMWebSocketClientSubsystem::OnMessage(const FString& Message)
{
}

void UMWebSocketClientSubsystem::OnMessageSent(const FString& Message)
{
}
