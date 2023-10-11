#pragma once

#include "GameRpcInterface.h"
#include "INetworkingWebSocket.h"
#include "RpcManager.h"

class FRole;

class FGameSession
{
	
public:

	explicit FGameSession(INetworkingWebSocket* InWebSocket);

	FGameSession(FGameSession&& Right) noexcept
		: ID(Right.ID)
	{
		WebSocket = Right.WebSocket;
		Right.WebSocket = nullptr;

		RpcInterface = MakeShared<FGameRpcInterface>(&Manager);
	}

	~FGameSession()
	{
		if (WebSocket)
		{
			WebSocket = nullptr;
		}
	}

	FGameSession(const FGameSession&) = delete;
	FGameSession& operator=(const FGameSession&) = delete;
	FGameSession& operator=(FGameSession&&) = delete;

	void Send(const TArray<uint8>& Data) const;
	
	void OnReceive(void* InData, const int32 Size);

	FString Account;
	
	FRole* Role = nullptr;

	TSharedPtr<FGameRpcInterface> RpcInterface;
	
	FServerRpcManager Manager;
	
	TSharedPtr<INetworkingWebSocket> WebSocket = nullptr;
	
	FGuid ID;
};

typedef TSharedPtr<FGameSession> FGameSessionPtr;