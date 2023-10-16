#include "GameRpcInterface.h"
#include "RpcManager.h"

FGameRpcInterface::FGameRpcInterface(FServerRpcManager* InManager)
{
    InManager->AddMethod(FLoginGameReq::KeyTypeID, [this, InManager](const FServerPtr& InConn, const FNetworkMessage& InMessage)
    {
        const uint64 ReqSerialNum = InMessage.SerialNum;

        const FLoginGameReq ReqMessage;
        FLoginGameAck RspMessage;
        ReqMessage.ParseFromArray(InMessage.GetBody());

        if (InMessage.GetBody().Num() <= 0)
		{
			InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::UnKnow);
            return;
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
