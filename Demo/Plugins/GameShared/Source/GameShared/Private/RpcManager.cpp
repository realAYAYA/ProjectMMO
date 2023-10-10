#include "RpcManager.h"

#include "IWebSocket.h"

static constexpr uint32 HeadLength = sizeof(FMRpcMessage);// 包头长度
static constexpr uint32 MaxBodyLength = 1024 * 1024 * 8;// 最大包体长度

void UMGameRpcManager::SendRequest(
	const TSharedPtr<IWebSocket>& InConn,
	const FMGameMessage& InMessage,
	const FResponseCallback& Callback)
{
	FMRpcMessage ReqMessage;
	
	ReqMessage.RpcMessageOp = ERpcMessageOp::Request;
	ReqMessage.SerialNum = ++SerialNum;
	ReqMessage.RpcErrorCode = ERpcErrorCode::Ok;
	ReqMessage.TypeID = InMessage.GetTypeID();
	InMessage.SerializeToArray(ReqMessage.SetBody());

	TArray<uint8> BinaryData;
	ReqMessage.SerializeToArray(BinaryData);

	if (!CheckBodyLength(BinaryData))
	{
		return;
	}
	
	InConn->Send(BinaryData.GetData(), BinaryData.Num(), true);

	FRequestPendingData Data;
	Data.SerialNum = SerialNum;
	Data.Callback = Callback;
	Data.ExpireTimestamp = 0;
	AllRequestPending.Emplace(SerialNum, std::move(Data));
}

void UMGameRpcManager::SendResponse(
	const TSharedPtr<IWebSocket>& InConn,
	const uint64 InReqSerialNum,
	const FMGameMessage& InMessage,
	const ERpcErrorCode ErrorCode)
{
	FMRpcMessage ReqMessage;
	
	ReqMessage.RpcMessageOp = ERpcMessageOp::Response;
	ReqMessage.SerialNum = InReqSerialNum;
	ReqMessage.RpcErrorCode = ErrorCode;
	ReqMessage.TypeID = InMessage.GetTypeID();
	InMessage.SerializeToArray(ReqMessage.SetBody());

	TArray<uint8> BinaryData;
	ReqMessage.SerializeToArray(BinaryData);

	if (!CheckBodyLength(BinaryData))
	{
		return;
	}
	
	InConn->Send(BinaryData.GetData(), BinaryData.Num(), true);
}

void UMGameRpcManager::AddMethod(uint64 InReqTypeId, const FMethodCallback& InCallback)
{
}

void UMGameRpcManager::OnMessage(
	const TSharedPtr<IWebSocket>& InConn,
	uint64 InCode,
	const char* InDataPtr,
	int32 InDataLen)
{
	
}

bool UMGameRpcManager::CheckBodyLength(const TArray<uint8>& Data)
{
	const uint32 TotalLength = Data.Num();
	const uint32 BodyLength = TotalLength - HeadLength;
	if (BodyLength > MaxBodyLength)
	{
		UE_LOG(LogTemp, Warning, TEXT("数据大小超标 %s / %s"), *FString::FromInt(BodyLength), *FString::FromInt(MaxBodyLength));
		
		return false;
	}
	
	return true;
}

void UMGameRpcManager::OnRpcMessage(const TSharedPtr<IWebSocket>& InConn, const FMRpcMessage& InMessage)
{
	switch (InMessage.RpcMessageOp)
	{
	case ERpcMessageOp::Notify:
		{
			OnNotify(InConn, InMessage);
		}
		break;
	case ERpcMessageOp::Request:
		{
			OnRequest(InConn, InMessage);
		}
		break;
	case ERpcMessageOp::Response:
		{
			OnResponse(InConn, InMessage);
		}
		break;
	default:
		break;
	}
}

void UMGameRpcManager::OnNotify(const TSharedPtr<IWebSocket>& InConn, const FMRpcMessage& InMessage)
{
}

// 服务器调用，收到请求
void UMGameRpcManager::OnRequest(const TSharedPtr<IWebSocket>& InConn, const FMRpcMessage& InMessage)
{
	const auto Ret = AllMethods.Find(InMessage.TypeID);
	if (!Ret)
		return;

	(*Ret)(InConn, InMessage);
}

// 客户端调用，收到服务器回应
void UMGameRpcManager::OnResponse(const TSharedPtr<IWebSocket>& InConn, const FMRpcMessage& InMessage)
{
	const auto Ret = AllRequestPending.Find(InMessage.TypeID);
	if (!Ret)
		return;

	Ret->Callback(InMessage.RpcErrorCode, InMessage);

	AllRequestPending.Remove(InMessage.TypeID);
}
