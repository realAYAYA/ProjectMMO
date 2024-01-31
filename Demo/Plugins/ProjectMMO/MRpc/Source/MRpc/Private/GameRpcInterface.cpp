#include "GameRpcInterface.h"
#include "RpcManager.h"

FGameRpcInterface::FGameRpcInterface(FServerRpcManager* InManager)
{
    InManager->AddMethod(FLoginAsDSReq::KeyTypeID, [this, InManager](const FServerPtr& InConn, const FNetworkMessage& InMessage)
    {
        const uint64 ReqSerialNum = InMessage.SerialNum;

        const FLoginAsDSReq ReqMessage;
        FLoginAsDSAck RspMessage;
        ReqMessage.ParseFromArray(InMessage.GetBody());

        if (InMessage.GetBody().Num() <= 0)
		{
			InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::UnKnow);
            return;
		}

        if (OnLoginAsDS)
        {
            OnLoginAsDS(ReqMessage, RspMessage);
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Ok);
        }
        else
        {
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Unimplemented);
        }
    });
    InManager->AddMethod(FPullRoleDataReq::KeyTypeID, [this, InManager](const FServerPtr& InConn, const FNetworkMessage& InMessage)
    {
        const uint64 ReqSerialNum = InMessage.SerialNum;

        const FPullRoleDataReq ReqMessage;
        FPullRoleDataAck RspMessage;
        ReqMessage.ParseFromArray(InMessage.GetBody());

        if (InMessage.GetBody().Num() <= 0)
		{
			InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::UnKnow);
            return;
		}

        if (OnPullRoleData)
        {
            OnPullRoleData(ReqMessage, RspMessage);
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Ok);
        }
        else
        {
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Unimplemented);
        }
    });
    InManager->AddMethod(FCommitRoleDataReq::KeyTypeID, [this, InManager](const FServerPtr& InConn, const FNetworkMessage& InMessage)
    {
        const uint64 ReqSerialNum = InMessage.SerialNum;

        const FCommitRoleDataReq ReqMessage;
        FCommitRoleDataAck RspMessage;
        ReqMessage.ParseFromArray(InMessage.GetBody());

        if (InMessage.GetBody().Num() <= 0)
		{
			InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::UnKnow);
            return;
		}

        if (OnCommitRoleData)
        {
            OnCommitRoleData(ReqMessage, RspMessage);
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Ok);
        }
        else
        {
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Unimplemented);
        }
    });
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
    InManager->AddMethod(FLogoutGameReq::KeyTypeID, [this, InManager](const FServerPtr& InConn, const FNetworkMessage& InMessage)
    {
        const uint64 ReqSerialNum = InMessage.SerialNum;

        const FLogoutGameReq ReqMessage;
        FLogoutGameAck RspMessage;
        ReqMessage.ParseFromArray(InMessage.GetBody());

        if (InMessage.GetBody().Num() <= 0)
		{
			InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::UnKnow);
            return;
		}

        if (OnLogoutGame)
        {
            OnLogoutGame(ReqMessage, RspMessage);
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Ok);
        }
        else
        {
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Unimplemented);
        }
    });
    InManager->AddMethod(FCreateRoleReq::KeyTypeID, [this, InManager](const FServerPtr& InConn, const FNetworkMessage& InMessage)
    {
        const uint64 ReqSerialNum = InMessage.SerialNum;

        const FCreateRoleReq ReqMessage;
        FCreateRoleAck RspMessage;
        ReqMessage.ParseFromArray(InMessage.GetBody());

        if (InMessage.GetBody().Num() <= 0)
		{
			InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::UnKnow);
            return;
		}

        if (OnCreateRole)
        {
            OnCreateRole(ReqMessage, RspMessage);
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Ok);
        }
        else
        {
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Unimplemented);
        }
    });
    InManager->AddMethod(FEnterWorldReq::KeyTypeID, [this, InManager](const FServerPtr& InConn, const FNetworkMessage& InMessage)
    {
        const uint64 ReqSerialNum = InMessage.SerialNum;

        const FEnterWorldReq ReqMessage;
        FEnterWorldAck RspMessage;
        ReqMessage.ParseFromArray(InMessage.GetBody());

        if (InMessage.GetBody().Num() <= 0)
		{
			InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::UnKnow);
            return;
		}

        if (OnEnterWorld)
        {
            OnEnterWorld(ReqMessage, RspMessage);
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Ok);
        }
        else
        {
            InManager->SendResponse(InConn, ReqSerialNum, RspMessage, ERpcErrorCode::Unimplemented);
        }
    });
}