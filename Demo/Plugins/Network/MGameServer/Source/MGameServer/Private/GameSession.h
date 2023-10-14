#pragma once

#include "GameRpcInterface.h"
#include "INetworkingWebSocket.h"
#include "RpcManager.h"

class FRole;

class FGameSession
{
	
public:

	explicit FGameSession(INetworkingWebSocket* InWebSocket, const FGuid& InID);

	FGameSession(FGameSession&& Right) noexcept;

	~FGameSession()
	{
		if (WebSocket)
		{
			WebSocket.Reset();
		}
	}

	FGameSession(const FGameSession&) = delete;
	FGameSession& operator=(const FGameSession&) = delete;
	FGameSession& operator=(FGameSession&&) = delete;

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

typedef TSharedPtr<FGameSession> FGameSessionPtr;