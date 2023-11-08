#pragma once

#include "GameRpcInterface.h"
#include "INetworkingWebSocket.h"
#include "RpcManager.h"
#include "MGameSession.generated.h"

class FMPlayer;

UCLASS()
class UMGameSession : public UObject
{
	GENERATED_BODY()
	
public:

	UMGameSession() {}
	
	void Initialize(INetworkingWebSocket* InWebSocket, const FGuid& InID);

	virtual void BeginDestroy() override;

	void Send(const TArray<uint8>& Data);

	void OnConnected();
	
	void OnReceive(void* InData, const int32 Size);

	void Shutdown() {}

	virtual void Offline() {}

	TSharedPtr<FGameRpcInterface> RpcInterface;
	
	FServerRpcManager Manager;
	
	TSharedPtr<INetworkingWebSocket> WebSocket = nullptr;
	
	FGuid ID;

	FDateTime GetLastSentTime() const { return LastSentTime.Load(); }
	FDateTime GetLastReceivedTime() const { return LastReceivedTime.Load(); }
	
	TAtomic<int64> LastSentTime{0};	// 最后发送数据时间
	TAtomic<int64> LastReceivedTime{0};// 最后接受数据时间
};
