#pragma once

#include "CoreMinimal.h"
#include "GameMessage.h"
#include "GameSession.h"
#include "MGameServerSubsystem.generated.h"

class IWebSocketServer;

DECLARE_DELEGATE_OneParam(FMWebSocketClientClosedCallBack, const FGuid);
DECLARE_DELEGATE_TwoParams(FMWebSocketReceiveCallBack, const FGuid, FString);

UCLASS()
class UMGameServerSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
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
	void StopWebSocketServer();

	void SendToAll(const FGameMessage& InMessage);
	
	void SendToAll(const TArray<uint8>& InData);

	void SendTo(const FGuid InID, const TArray<uint8>& InData);
	
	void SendTo(const FGameSessionPtr InConn, const TArray<uint8>& InData);
	
	bool CheckConnectionValid(const FGuid InID);

	FMWebSocketClientClosedCallBack WebSocketClientClosedCallBack;
	FMWebSocketReceiveCallBack WebSocketReceiveCallBack;

protected:

	void OnClientConnected(INetworkingWebSocket* InWebSocket);
	bool IsServerRunning() const;

	void OnConnected(const FGuid InID) const;
	//void OnReceive(void* InData, const int32 DataSize, const FGuid InID);
	void OnError(const FGuid InID);
	void OnClosed(const FGuid InID);

private:

	TUniquePtr<IWebSocketServer> WebSocketServer;

	TMap<FGuid, FGameSessionPtr> Connections;
};
