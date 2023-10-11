#pragma once

#include "INetworkingWebSocket.h"

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
	}

	~FGameSession()
	{
		if (WebSocket)
		{
			delete WebSocket;
			WebSocket = nullptr;
		}
	}

	FGameSession(const FGameSession&) = delete;
	FGameSession& operator=(const FGameSession&) = delete;
	FGameSession& operator=(FGameSession&&) = delete;

	void Send(const TArray<uint8>& Data) const;
	
	void OnReceive(void* InData, const int32 DataSize);

	FString Account;
	
	FRole* Role = nullptr;

	INetworkingWebSocket* WebSocket = nullptr;
	FGuid ID;
};

typedef TSharedPtr<FGameSession> FGameSessionPtr;