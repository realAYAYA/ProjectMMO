#pragma once

#include "GameRpcInterface.h"
#include "INetworkingWebSocket.h"
#include "RpcManager.h"
#include "MGameSession.generated.h"

class UMWorld;
class UMPlayer;
class FMPlayer;

UCLASS()
class UMGameSession : public UObject
{
	GENERATED_BODY()
	
public:

	UMGameSession() {}
	
	void Initialize(INetworkingWebSocket* InWebSocket, const FGuid& InID);
	void Shutdown();

	virtual void BeginDestroy() override;

	void Send(const FGameMessage& InMessage);
	void Send(const TArray<uint8>& Data);

	void OnConnected();
	
	void OnReceive(void* InData, const int32 Size);
	
	virtual void Offline() {}

	UPROPERTY()
	UMPlayer* Player = nullptr;

	UPROPERTY()
	UMWorld* World = nullptr;

	TSharedPtr<FGameRpcInterface> RpcInterface;
	
	FServerRpcManager Manager;
	
	TSharedPtr<INetworkingWebSocket> WebSocket = nullptr;
	
	FGuid ID;

	FDateTime GetLastSentTime() const { return LastSentTime.Load(); }
	FDateTime GetLastReceivedTime() const { return LastReceivedTime.Load(); }

private:
	
	TAtomic<int64> LastSentTime{0};	// 最后发送数据时间
	TAtomic<int64> LastReceivedTime{0};// 最后接受数据时间
};
