#pragma once

#include "GameRpcInterface.h"
#include "INetworkingWebSocket.h"
#include "RpcManager.h"
#include "MGameSession.generated.h"

class FRole;

UCLASS()
class UMGameSession final : public UObject
{
	GENERATED_BODY()
	
public:

	UMGameSession() {}
	
	void Initialize(INetworkingWebSocket* InWebSocket, const FGuid& InID);

	virtual void BeginDestroy() override;

	void Send(const TArray<uint8>& Data) const;

	void OnConnected();
	
	void OnReceive(void* InData, const int32 Size);

	FString Account;
	
	FRole* Role = nullptr;

	TSharedPtr<FGameRpcInterface> RpcInterface;
	
	FServerRpcManager Manager;
	
	TSharedPtr<INetworkingWebSocket> WebSocket = nullptr;
	
	FGuid ID;
};