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

/*
void FClientRpcManager::Tick(float DeltaTime)
{
#if !WITH_EDITOR
	// 编辑器模式下不进行超时计算
	const FDateTime Now = FDateTime::UtcNow();
	if ((Now - LastCheckAliveTime).GetTotalSeconds() > 8)
	{
		TArray<uint64> Keys;
		for (const auto& Tuple : AllRequestPending)
		{
			const FRequestPendingData& PendingData = Tuple.Value;
			if ((Now - FDateTime(PendingData.ExpireTimestamp)).GetTotalSeconds() > 10)
			{
				Keys.Add(Tuple.Key);

				// 触发超时
				PendingData.Callback(ERpcErrorCode::TimeOut, FNetworkMessage());
			}
		}

		for (const uint64 Key : Keys)
		{
			AllRequestPending.Remove(Key);
		}
		
		LastCheckAliveTime = Now;
	}
#endif
}

bool FClientRpcManager::IsTickable() const
{
	return true;
}

TStatId FClientRpcManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FClientRpcManager, STATGROUP_Tickables);
}
*/

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
	
	InConn->Send(BinaryData.GetData(), BinaryData.Num(), false);

	FRequestPendingData Data;
	Data.SerialNum = SerialNum;
	Data.Callback = Callback;
	Data.ExpireTimestamp = FDateTime::UtcNow().GetTicks();
	AllRequestPending.Emplace(SerialNum, std::move(Data));
}

void FClientRpcManager::DispatchNotify(
	const FConnectionPtr& InConn,
	const uint64 TypeID,
	const FNotifyCallback& Callback)
{
	FNotifyPendingData Data;
	Data.Callback = Callback;

	AllNotifyPending.Emplace(TypeID, std::move(Data));
}

void FClientRpcManager::OnMessage(const FConnectionPtr& InConn, const FNetworkMessage& InMessage)
{
	switch (InMessage.RpcMessageOp)
	{
	case ERpcMessageOp::Notify:
		{
			const auto Ret = AllNotifyPending.Find(InMessage.TypeID);
			if (!Ret)
				return;

			Ret->Callback(InMessage);
		}
		break;
	case ERpcMessageOp::Request:
		{
			
		}
		break;
	case ERpcMessageOp::Response:
		{
			const auto Ret = AllRequestPending.Find(InMessage.SerialNum);
			if (!Ret)
				return;

			Ret->Callback(InMessage.RpcErrorCode, InMessage);

			AllRequestPending.Remove(InMessage.SerialNum);
		}
		break;
	default:
		break;
	}
}

void FServerRpcManager::SendNotify(const FServerPtr& InConn, const FGameMessage& InMessage)
{
	FNetworkMessage ReqMessage;
	ReqMessage.RpcErrorCode = ERpcErrorCode::Ok;
	ReqMessage.RpcMessageOp = ERpcMessageOp::Notify;
	ReqMessage.TypeID = InMessage.GetTypeID();
	InMessage.SerializeToArray(ReqMessage.SetBody());

	TArray<uint8> BinaryData;
	ReqMessage.SerializeToArray(BinaryData);

	if (!CheckBodyLength(BinaryData))
	{
		return;
	}
	
	InConn->Send(BinaryData.GetData(), BinaryData.Num() * sizeof(uint8), false);
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
	
	InConn->Send(BinaryData.GetData(), BinaryData.Num() * sizeof(uint8), false);
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