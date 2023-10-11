#pragma once

#include "CoreMinimal.h"
#include "MRpcMessage.h"
#include "NetFwd.h"
#include "RpcManager.generated.h"

UCLASS()
class MRPC_API URpcManager : public UObject
{
	GENERATED_BODY()
	
public:

	typedef TFunction<void(const FConnectionPtr& InConn, const FMRpcMessage&)> FMethodCallback;
	typedef TFunction<void(ERpcErrorCode, const FMRpcMessage&)> FResponseCallback;

	void SendRequest(const FConnectionPtr& InConn, const FMGameMessage& InMessage, const FResponseCallback& Callback);
	static void SendResponse(const FConnectionPtr& InConn, const uint64 InReqSerialNum, const FMGameMessage& InMessage, const ERpcErrorCode ErrorCode);

	void AddMethod(uint64 InReqTypeId, const FMethodCallback& InCallback);
	
	void OnMessage(const FConnectionPtr& InConn, uint64 InCode, const char* InDataPtr, int32 InDataLen);

	static bool CheckBodyLength(const TArray<uint8>& Data);
	
private:

	void OnRpcMessage(const FConnectionPtr& InConn, const FMRpcMessage& InMessage);
	void OnNotify(const FConnectionPtr& InConn, const FMRpcMessage& InMessage);
	void OnRequest(const FConnectionPtr& InConn, const FMRpcMessage& InMessage);
	void OnResponse(const FConnectionPtr& InConn, const FMRpcMessage& InMessage);
	
	uint64 SerialNum = 1;

	struct FRequestPendingData
	{
		uint64 SerialNum = 0;
		FResponseCallback Callback;
		int64 ExpireTimestamp = 0;
	};
	
	TMap<uint64, FRequestPendingData> AllRequestPending;
	
	TMap<uint64, FMethodCallback> AllMethods;
	
};
