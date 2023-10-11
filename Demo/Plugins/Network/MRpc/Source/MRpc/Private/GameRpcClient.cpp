#include "GameRpcClient.h"

void UGameRpcClient::Setup(FRpcManager* InManager, const FConnectionPtr& InConn)
{
	Manager = InManager;
	Connection = InConn;

	
}

void UGameRpcClient::K2_LoginGame(const FLoginReq& InParams, const FOnLoginResult& InCallback)
{
	if (!Manager || !Connection)
		return;
        
	auto ReqMessage = MakeShared<idlezt::LoginGameReq>();
	InParams.ToPb(&ReqMessage.Get());  // USTRUCT -> PB
    
	LoginGame(ReqMessage, [InCallback](EZRpcErrorCode ErrorCode, const TSharedPtr<idlezt::LoginGameAck>& InRspMessage)
	{        
		const UObject* Owner = InCallback.GetUObject();
		if (IsValid(Owner))
		{
			FZLoginGameAck Rsp;
			if (InRspMessage)
			{
				Rsp = *InRspMessage;  // PB -> USTRUCT
			}        

			InCallback.ExecuteIfBound(ErrorCode, Rsp);
		}
	});
}

void UGameRpcClient::LoginGame(const FLoginReq& InReqMessage, const OnLoginGameResult& InCallback)
{
	if (!Manager || !Connection)
		return;

	Manager->SendRequest(Connection, InReqMessage, [InCallback](EZRpcErrorCode ErrorCode, const TSharedPtr<idlezt::ZRpcMessage>& InMessage)
	{
		auto RspMessage = MakeShared<idlezt::LoginGameAck>();
               
		if (ErrorCode == EZRpcErrorCode::RpcErrorCode_Ok)
		{
			if (!RspMessage->ParseFromArray(InMessage->body().c_str(), InMessage->body().size()))
				return;
		}

		InCallback(ErrorCode, RspMessage);
	});
}
