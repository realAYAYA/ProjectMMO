#pragma once

#include "CoreMinimal.h"

#include "IWebSocket.h"
#include "MGameClientSubsystem.generated.h"

UCLASS()
class UMWebSocketClientSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
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
	void CreateSocket();

	UFUNCTION(BlueprintCallable, Category = "MWebSocketServer")
	void CloseSocket();

	UFUNCTION(BlueprintCallable, Category = "MWebSocketServer")
	void SendMessage(const FString& InMessage);

	//FMWebSocketClientClosedCallBack WebSocketClientClosedCallBack;
	//FMWebSocketReceiveCallBack WebSocketReceiveCallBack;

protected:
	
	void OnConnected() const;
	void OnConnectionError(const FString& Error);
	void OnClosed(const int32 StatusCode, const FString& Reason, const bool bWasClean);
	void OnMessage(const FString& Message);
	void OnMessageSent(const FString& Message);

private:

	TSharedPtr<IWebSocket> Socket;

};
