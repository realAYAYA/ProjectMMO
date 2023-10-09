#pragma once

#include "CoreMinimal.h"
#include "MRpcMessage.h"
#include "RpcManager.generated.h"

class IWebSocket;

UCLASS()
class MGAMECLIENT_API UMGameRpcManager : public UObject
{
	GENERATED_BODY()
	
public:

	typedef TFunction<void(const TSharedPtr<IWebSocket>&, const FMGameRpcMessage&)> FMethodCallback;
	typedef TFunction<void(ERpcErrorCode, const FMGameRpcMessage&)> FResponseCallback;

	void SendRequest(const TSharedPtr<IWebSocket>& InConn, const FMGameMessage& InMessage, const FResponseCallback& Callback);
	void SendResponse(const TSharedPtr<IWebSocket>& InConn, uint64 InReqSerialNum, const FMGameMessage& InMessage, ERpcErrorCode ErrorCode);

	void AddMethod(uint64 InReqTypeId, const FMethodCallback& InCallback);
	
	void OnMessage(const TSharedPtr<IWebSocket>& InConn, uint64 InCode, const char* InDataPtr, int32 InDataLen);
	
private:

	void OnRpcMessage(const TSharedPtr<IWebSocket>& InConn, const FMGameRpcMessage& InMessage);
	void OnNotify(const TSharedPtr<IWebSocket>& InConn, const FMGameRpcMessage& InMessage);
	void OnRequest(const TSharedPtr<IWebSocket>& InConn, const FMGameRpcMessage& InMessage);
	void OnResponse(const TSharedPtr<IWebSocket>& InConn, const FMGameRpcMessage& InMessage);
	
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
