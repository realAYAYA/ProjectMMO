
#include "GameRpcClient.h"
#include "RpcManager.h"

void UGameRpcClient::Setup(FClientRpcManager* InManager, const FConnectionPtr& InConn)
{
    Manager = InManager;
    Connection = InConn;
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
        else
		{
			UE_LOG(LogGameNetwork, Warning, TEXT("%s : InValid Callback"), *FString(__FUNCTION__));
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


