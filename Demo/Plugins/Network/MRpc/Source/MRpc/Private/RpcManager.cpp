#include "RpcManager.h"

#include "INetworkingWebSocket.h"
#include "IWebSocket.h"

DEFINE_LOG_CATEGORY(LogGameNetwork);

static constexpr uint32 HeadLength = sizeof(FNetworkMessage);// 包头长度
static constexpr uint32 MaxBodyLength = 1024 * 1024 * 8;// 最大包体长度

bool CheckBodyLength(const TArray<uint8>& Data)
{
	const uint32 TotalLength = Data.Num();
	const uint32 BodyLength = TotalLength - HeadLength;
	if (BodyLength > MaxBodyLength)
	{
		UE_LOG(LogGameNetwork, Warning, TEXT("数据大小超标 %s / %s"), *FString::FromInt(BodyLength), *FString::FromInt(MaxBodyLength));
		
		return false;
	}
	
	return true;
}

void FClientRpcManager::SendRequest(
	const FConnectionPtr& InConn,
	const FGameMessage& InMessage,
	const FResponseCallback& Callback)
{
	FNetworkMessage ReqMessage;

	ReqMessage.TypeID = InMessage.GetTypeID();
	ReqMessage.RpcMessageOp = ERpcMessageOp::Request;
	ReqMessage.RpcErrorCode = ERpcErrorCode::Ok;
	ReqMessage.SerialNum = ++SerialNum;
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

void FClientRpcManager::OnMessage(const FConnectionPtr& InConn, const FNetworkMessage& InMessage)
{
	switch (InMessage.RpcMessageOp)
	{
	case ERpcMessageOp::Notify:
		{
			
		}
		break;
	case ERpcMessageOp::Request:
		{
			
		}
		break;
	case ERpcMessageOp::Response:
		{
			const auto Ret = AllRequestPending.Find(InMessage.TypeID);
			if (!Ret)
				return;

			Ret->Callback(InMessage.RpcErrorCode, InMessage);

			AllRequestPending.Remove(InMessage.TypeID);
		}
		break;
	default:
		break;
	}
}

void FServerRpcManager::SendResponse(
	const FServerPtr& InConn,
	const uint64 InReqSerialNum,
	const FGameMessage& InMessage,
	const ERpcErrorCode ErrorCode)
{
	FNetworkMessage ReqMessage;
	
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
	
	InConn->Send(BinaryData.GetData(), BinaryData.Num(), false);
}

void FServerRpcManager::AddMethod(uint64 InReqTypeId, const FMethodCallback& InCallback)
{
	Methods.Emplace(InReqTypeId, InCallback);
}

void FServerRpcManager::OnMessage(const FServerPtr& InConn, const FNetworkMessage& InMessage)
{
	switch (InMessage.RpcMessageOp)
	{
	case ERpcMessageOp::Notify:
		{
			
		}
		break;
	case ERpcMessageOp::Request:
		{
			const auto Ret = Methods.Find(InMessage.TypeID);
			if (!Ret)
				return;

			(*Ret)(InConn, InMessage);
		}
		break;
	case ERpcMessageOp::Response:
		{
		}
		break;
	default:
		break;
	}
}