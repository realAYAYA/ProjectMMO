#include "GameRpcInterface.h"
#include "RpcManager.h"

FGameRpcInterface::FGameRpcInterface(FServerRpcManager* InManager)
{
	InManager->AddMethod(FLoginReq::KeyTypeID, [this, InManager](const FServerPtr& InConn, const FNetworkMessage& InMessage)
	{
		const uint64 ReqSerialNum = InMessage.TypeID;

		const FLoginReq ReqMessage;
		FLoginAck RspMessage;
		ReqMessage.ParseFromArray(InMessage.GetBody());

		if (InMessage.GetBody().Num() <= 0)
		{
			InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::UnKnow);
		}

		if (OnLoginGame)
		{
			OnLoginGame(ReqMessage, RspMessage);
			InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Ok);
		}
		else
		{
			InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Unimplemented);
		}
	});
}

FGameRpcInterface::~FGameRpcInterface()
{
}
