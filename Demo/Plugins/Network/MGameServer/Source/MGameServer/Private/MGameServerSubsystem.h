#pragma once

#include "CoreMinimal.h"
#include "IWebSocketServer.h"

#include "GameMessage.h"
#include "MGameSession.h"
#include "MGameServerSubsystem.generated.h"

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

	// Test
	UFUNCTION(BlueprintCallable, Category = "ProjcetM", DisplayName = "StartWebSocketServer")
	bool K2_StartWebSocketServer(const int32 ServerPort = 10086);

	// Test
	UFUNCTION(BlueprintCallable, Category = "ProjcetM", DisplayName = "SendMessageToAll")
	void K2_SendToAll(const FString& InMessage);
	
	bool StartWebSocketServer(const int32 ServerPort);

	UFUNCTION(BlueprintCallable, Category = "MWebSocketServer")
	void StopWebSocketServer();

	void SendToAll(const FGameMessage& InMessage);
	
	void SendToAll(const TArray<uint8>& InData);
	
	bool CheckConnectionValid(const FGuid InID);

	FMWebSocketClientClosedCallBack WebSocketClientClosedCallBack;
	FMWebSocketReceiveCallBack WebSocketReceiveCallBack;

protected:

	void OnClientConnected(INetworkingWebSocket* InWebSocket);
	bool IsServerRunning() const;

	void OnConnected(const FGuid InID);
	void OnReceive(void* InData, const int32 DataSize, const FGuid InID);
	void OnError(const FGuid InID);
	void OnClosed(const FGuid InID);

private:

	TUniquePtr<IWebSocketServer> WebSocketServer;

	UPROPERTY()
	TMap<FGuid, UMGameSession*> Connections;
};
