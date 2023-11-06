
#include "GameRpcClient.h"
#include "RpcManager.h"

void UGameRpcClient::Setup(FClientRpcManager* InManager, const FConnectionPtr& InConn)
{
    Manager = InManager;
    Connection = InConn;

    if (Manager)
    {
        Manager->DispatchNotify(InConn, FUpdateChatMessage::KeyTypeID, [this](const FNetworkMessage& InMessage)
        {
            if (OnUpdateChatMessage.IsBound())
            {
                const FUpdateChatMessage NotifyMessage;
                NotifyMessage.ParseFromArray(InMessage.GetBody());
                OnUpdateChatMessage.Broadcast(NotifyMessage);
            }
        });
    }
}


void UGameRpcClient::K2_LoginGame(const FLoginGameReq& InParams, const FOnLoginGameResult& InCallback)
{
    if (!Manager || !Connection)
        return;

    LoginGame(InParams, [InCallback](const ERpcErrorCode ErrorCode, const FLoginGameAck& InRspMessage)
    {
        if (IsValid(InCallback.GetUObject()))
        {
            InCallback.Execute(ErrorCode, InRspMessage);
        }
    });
}

void UGameRpcClient::LoginGame(const FLoginGameReq& InReqMessage, const FOnLoginGameResultFunction& InCallback) const
{   
    if (!Manager || !Connection)
        return;

    Manager->SendRequest(Connection, InReqMessage, [InCallback](const ERpcErrorCode ErrorCode, const FNetworkMessage& InMessage)
    {
        const FLoginGameAck RspMessage;

        if (ErrorCode == ERpcErrorCode::Ok)
        {
            RspMessage.ParseFromArray(InMessage.GetBody());
        }

        InCallback(ErrorCode, RspMessage);
    });
}


