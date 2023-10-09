#include "RpcManager.h"

#include "IWebSocket.h"

void UMGameRpcManager::SendRequest(
	const TSharedPtr<IWebSocket>& InConn,
	const FMGameMessage& InMessage,
	const FResponseCallback& Callback)
{
	FMGameRpcMessage ReqMessage;

	// Todo 将InMessage序列化为TArray<uint8> Data, ReqMessage.Body = Data
	ReqMessage.RpcMessageOp = ERpcMessageOp::Request;
	ReqMessage.SerialNum = ++SerialNum;
	ReqMessage.RpcErrorCode = ERpcErrorCode::Ok;
	ReqMessage.TypeID = InMessage.GetTypeID();
	//Todo ReqMessage.SetBody();

	//Todo 发送流程，检查包体大小，序列化成功判定 
	// InConn->Send(Message);

	FRequestPendingData Data;
	Data.SerialNum = SerialNum;
	Data.Callback = Callback;
	Data.ExpireTimestamp = 0;
	AllRequestPending.Emplace(SerialNum, std::move(Data));
}

void UMGameRpcManager::SendResponse(
	const TSharedPtr<IWebSocket>& InConn,
	uint64 InReqSerialNum,
	const FMGameMessage& InMessage,
	ERpcErrorCode ErrorCode)
{
	/*const int32 BodySize = InMessage->ByteSizeLong();
	TArray<char> Body;
	Body.SetNumUninitialized(BodySize);

	if (!InMessage->SerializeToArray(Body.GetData(), BodySize))
	{
		return;
	}
		
	auto Message = MakeShared<idlezt::ZRpcMessage>();
	Message->set_op(idlezt::RpcMessageOp_Response);
	Message->set_sn(InReqSerialNum);
	Message->set_error_code(ErrorCode);
	Message->set_type_id(InMessage->GetTypeId());
	Message->set_body(Body.GetData(), BodySize);
	InConn->Send(Message);*/
}

void UMGameRpcManager::AddMethod(uint64 InReqTypeId, const FMethodCallback& InCallback)
{
}

void UMGameRpcManager::OnMessage(const TSharedPtr<IWebSocket>& InConn, uint64 InCode, const char* InDataPtr,
	int32 InDataLen)
{
}

void UMGameRpcManager::OnRpcMessage(const TSharedPtr<IWebSocket>& InConn, const FMGameRpcMessage& InMessage)
{
}

void UMGameRpcManager::OnNotify(const TSharedPtr<IWebSocket>& InConn, const FMGameRpcMessage& InMessage)
{
}

void UMGameRpcManager::OnRequest(const TSharedPtr<IWebSocket>& InConn, const FMGameRpcMessage& InMessage)
{
}

void UMGameRpcManager::OnResponse(const TSharedPtr<IWebSocket>& InConn, const FMGameRpcMessage& InMessage)
{
}
