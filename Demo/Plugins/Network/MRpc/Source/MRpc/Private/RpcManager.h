#pragma once

#include "CoreMinimal.h"
#include "MRpcMessage.h"
#include "RpcManager.generated.h"

class IWebSocket;

UCLASS()
class MRPC_API UMGameRpcManager : public UObject
{
	GENERATED_BODY()
	
public:

	typedef TFunction<void(const TSharedPtr<IWebSocket>&, const FMRpcMessage&)> FMethodCallback;
	typedef TFunction<void(ERpcErrorCode, const FMRpcMessage&)> FResponseCallback;

	void SendRequest(const TSharedPtr<IWebSocket>& InConn, const FMGameMessage& InMessage, const FResponseCallback& Callback);
	static void SendResponse(const TSharedPtr<IWebSocket>& InConn, const uint64 InReqSerialNum, const FMGameMessage& InMessage, const ERpcErrorCode ErrorCode);

	void AddMethod(uint64 InReqTypeId, const FMethodCallback& InCallback);
	
	void OnMessage(const TSharedPtr<IWebSocket>& InConn, uint64 InCode, const char* InDataPtr, int32 InDataLen);

	static bool CheckBodyLength(const TArray<uint8>& Data);
	
private:

	void OnRpcMessage(const TSharedPtr<IWebSocket>& InConn, const FMRpcMessage& InMessage);
	void OnNotify(const TSharedPtr<IWebSocket>& InConn, const FMRpcMessage& InMessage);
	void OnRequest(const TSharedPtr<IWebSocket>& InConn, const FMRpcMessage& InMessage);
	void OnResponse(const TSharedPtr<IWebSocket>& InConn, const FMRpcMessage& InMessage);
	
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
