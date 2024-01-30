
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


void UGameRpcClient::K2_LoginAsDS(const FLoginAsDSReq& InParams, const FOnLoginAsDSResult& InCallback)
{
    if (!Manager || !Connection)
        return;

    LoginAsDS(InParams, [InCallback](const ERpcErrorCode ErrorCode, const FLoginAsDSAck& InRspMessage)
    {
        if (IsValid(InCallback.GetUObject()))
        {
            InCallback.Execute(ErrorCode, InRspMessage);
        }
    });
}

void UGameRpcClient::LoginAsDS(const FLoginAsDSReq& InReqMessage, const FOnLoginAsDSResultFunction& InCallback) const
{   
    if (!Manager || !Connection)
        return;

    Manager->SendRequest(Connection, InReqMessage, [InCallback](const ERpcErrorCode ErrorCode, const FNetworkMessage& InMessage)
    {
        const FLoginAsDSAck RspMessage;

        if (ErrorCode == ERpcErrorCode::Ok)
        {
            RspMessage.ParseFromArray(InMessage.GetBody());
        }

        InCallback(ErrorCode, RspMessage);
    });
}


void UGameRpcClient::K2_PullRoleData(const FPullRoleDataReq& InParams, const FOnPullRoleDataResult& InCallback)
{
    if (!Manager || !Connection)
        return;

    PullRoleData(InParams, [InCallback](const ERpcErrorCode ErrorCode, const FPullRoleDataAck& InRspMessage)
    {
        if (IsValid(InCallback.GetUObject()))
        {
            InCallback.Execute(ErrorCode, InRspMessage);
        }
    });
}

void UGameRpcClient::PullRoleData(const FPullRoleDataReq& InReqMessage, const FOnPullRoleDataResultFunction& InCallback) const
{   
    if (!Manager || !Connection)
        return;

    Manager->SendRequest(Connection, InReqMessage, [InCallback](const ERpcErrorCode ErrorCode, const FNetworkMessage& InMessage)
    {
        const FPullRoleDataAck RspMessage;

        if (ErrorCode == ERpcErrorCode::Ok)
        {
            RspMessage.ParseFromArray(InMessage.GetBody());
        }

        InCallback(ErrorCode, RspMessage);
    });
}


void UGameRpcClient::K2_CommitRoleData(const FCommitRoleDataReq& InParams, const FOnCommitRoleDataResult& InCallback)
{
    if (!Manager || !Connection)
        return;

    CommitRoleData(InParams, [InCallback](const ERpcErrorCode ErrorCode, const FCommitRoleDataAck& InRspMessage)
    {
        if (IsValid(InCallback.GetUObject()))
        {
            InCallback.Execute(ErrorCode, InRspMessage);
        }
    });
}

void UGameRpcClient::CommitRoleData(const FCommitRoleDataReq& InReqMessage, const FOnCommitRoleDataResultFunction& InCallback) const
{   
    if (!Manager || !Connection)
        return;

    Manager->SendRequest(Connection, InReqMessage, [InCallback](const ERpcErrorCode ErrorCode, const FNetworkMessage& InMessage)
    {
        const FCommitRoleDataAck RspMessage;

        if (ErrorCode == ERpcErrorCode::Ok)
        {
            RspMessage.ParseFromArray(InMessage.GetBody());
        }

        InCallback(ErrorCode, RspMessage);
    });
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


void UGameRpcClient::K2_LogoutGame(const FLogoutGameReq& InParams, const FOnLogoutGameResult& InCallback)
{
    if (!Manager || !Connection)
        return;

    LogoutGame(InParams, [InCallback](const ERpcErrorCode ErrorCode, const FLogoutGameAck& InRspMessage)
    {
        if (IsValid(InCallback.GetUObject()))
        {
            InCallback.Execute(ErrorCode, InRspMessage);
        }
    });
}

void UGameRpcClient::LogoutGame(const FLogoutGameReq& InReqMessage, const FOnLogoutGameResultFunction& InCallback) const
{   
    if (!Manager || !Connection)
        return;

    Manager->SendRequest(Connection, InReqMessage, [InCallback](const ERpcErrorCode ErrorCode, const FNetworkMessage& InMessage)
    {
        const FLogoutGameAck RspMessage;

        if (ErrorCode == ERpcErrorCode::Ok)
        {
            RspMessage.ParseFromArray(InMessage.GetBody());
        }

        InCallback(ErrorCode, RspMessage);
    });
}


void UGameRpcClient::K2_CreateRole(const FCreateRoleReq& InParams, const FOnCreateRoleResult& InCallback)
{
    if (!Manager || !Connection)
        return;

    CreateRole(InParams, [InCallback](const ERpcErrorCode ErrorCode, const FCreateRoleAck& InRspMessage)
    {
        if (IsValid(InCallback.GetUObject()))
        {
            InCallback.Execute(ErrorCode, InRspMessage);
        }
    });
}

void UGameRpcClient::CreateRole(const FCreateRoleReq& InReqMessage, const FOnCreateRoleResultFunction& InCallback) const
{   
    if (!Manager || !Connection)
        return;

    Manager->SendRequest(Connection, InReqMessage, [InCallback](const ERpcErrorCode ErrorCode, const FNetworkMessage& InMessage)
    {
        const FCreateRoleAck RspMessage;

        if (ErrorCode == ERpcErrorCode::Ok)
        {
            RspMessage.ParseFromArray(InMessage.GetBody());
        }

        InCallback(ErrorCode, RspMessage);
    });
}


void UGameRpcClient::K2_EnterWorld(const FEnterWorldReq& InParams, const FOnEnterWorldResult& InCallback)
{
    if (!Manager || !Connection)
        return;

    EnterWorld(InParams, [InCallback](const ERpcErrorCode ErrorCode, const FEnterWorldAck& InRspMessage)
    {
        if (IsValid(InCallback.GetUObject()))
        {
            InCallback.Execute(ErrorCode, InRspMessage);
        }
    });
}

void UGameRpcClient::EnterWorld(const FEnterWorldReq& InReqMessage, const FOnEnterWorldResultFunction& InCallback) const
{   
    if (!Manager || !Connection)
        return;

    Manager->SendRequest(Connection, InReqMessage, [InCallback](const ERpcErrorCode ErrorCode, const FNetworkMessage& InMessage)
    {
        const FEnterWorldAck RspMessage;

        if (ErrorCode == ERpcErrorCode::Ok)
        {
            RspMessage.ParseFromArray(InMessage.GetBody());
        }

        InCallback(ErrorCode, RspMessage);
    });
}


