#pragma once

#include "CoreMinimal.h"
#include "GameMessage.h"
#include "NetFwd.h"

class MRPC_API FClientRpcManager
{
	
public:
	
	typedef TFunction<void(const ERpcErrorCode, const FNetworkMessage&)> FResponseCallback;
	void SendRequest(const FConnectionPtr& InConn, const FGameMessage& InMessage, const FResponseCallback& Callback);

	typedef TFunction<void(const FNetworkMessage&)> FNotifyCallback;
	void DispatchNotify(const FConnectionPtr& InConn, const uint64 TypeID, const FNotifyCallback& Callback);

	void OnMessage(const FConnectionPtr& InConn, const FNetworkMessage& InMessage);
	
private:

	// Rpc
	uint64 SerialNum = 1;

	struct FRequestPendingData
	{
		uint64 SerialNum = 0;
		FResponseCallback Callback;
		int64 ExpireTimestamp = 0;
	};
	
	TMap<uint64, FRequestPendingData> AllRequestPending;

	FDateTime LastCheckAliveTime;

	// Notify message from server
	struct FNotifyPendingData
	{
		FNotifyCallback Callback;
	};
	
	TMap<uint64, FNotifyPendingData> AllNotifyPending;
};


class MRPC_API FServerRpcManager
{
	
public:

	static void SendNotify(const FServerPtr& InConn, const FGameMessage& InMessage);
	
	typedef TFunction<void(const FServerPtr& InConn, const FNetworkMessage&)> FMethodCallback;
	static void SendResponse(const FServerPtr& InConn, const uint64 InReqSerialNum, const FGameMessage& InMessage, const ERpcErrorCode ErrorCode);

	void AddMethod(uint64 InReqTypeId, const FMethodCallback& InCallback);

	void OnMessage(const FServerPtr& InConn, const FNetworkMessage& InMessage);
	
private:
	
	TMap<uint64, FMethodCallback> Methods;
};