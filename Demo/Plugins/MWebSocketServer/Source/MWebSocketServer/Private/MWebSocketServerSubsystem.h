#pragma once

#include "CoreMinimal.h"

#include "INetworkingWebSocket.h"
#include "MWebSocketServerSubsystem.generated.h"

class IWebSocketServer;

struct FMWebSocketConnection
{
	explicit FMWebSocketConnection(INetworkingWebSocket* InWebSocket)
		: WebSocket(InWebSocket), ID(FGuid::NewGuid())
	{
	}

	FMWebSocketConnection(FMWebSocketConnection&& Right) noexcept
		: ID(Right.ID)
	{
		WebSocket = Right.WebSocket;
		Right.WebSocket = nullptr;
	}

	~FMWebSocketConnection()
	{
		if (WebSocket)
		{
			delete WebSocket;
			WebSocket = nullptr;
		}
	}

	FMWebSocketConnection(const FMWebSocketConnection&) = delete;
	FMWebSocketConnection& operator=(const FMWebSocketConnection&) = delete;
	FMWebSocketConnection& operator=(FMWebSocketConnection&&) = delete;

	INetworkingWebSocket* WebSocket = nullptr;
	FGuid ID;
};

UCLASS()
class UMWebSocketServerSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

public:

	UFUNCTION(BlueprintCallable, Category = "MWebSocketServer")
	bool StartWebSocketServer(const int32 ServerPort);

	UFUNCTION(BlueprintCallable, Category = "MWebSocketServer")
	void StopWebSocketServer(const int32 ServerPort);

	UFUNCTION(BlueprintCallable, Category = "MWebSocketServer")
	void SendToAll(const int32 InMessage);
	
	void SendTo(const FGuid InID, const int32 InMessage);
	
	bool CheckConnectionValid(const FGuid InID);

protected:

	void OnClientConnected(INetworkingWebSocket* InWebSocket);
	bool IsServerRunning() const;

	void OnConnected(const FGuid InID);
	void OnReceive(const FGuid InID, void* Data);
	void OnError(const FGuid InID);
	void OnClosed(const FGuid InID);

	void SendToPointedClient(const FGuid& InClientID, const TArray<uint8>& InUTF8Payload);
	void ProcessAllClientInfo(const FGuid ClientID, const int32 Info);

private:

	TUniquePtr<IWebSocketServer> MWebSocketServer;

	TArray<FMWebSocketConnection> Connections;

};
