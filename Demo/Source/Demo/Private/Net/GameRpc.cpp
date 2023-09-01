#include "GameRpc.h"

void UGameRpc::K2_LoginGame(const FLoginGameReq& Req, const FOnRpcResult& InCallback)
{
	LoginGame(Req);
	RpcCallbackPool.Add(TEXT("Login"), &InCallback);
}

void UGameRpc::LoginGame_Implementation(const FLoginGameReq& Req)
{
	// 执行在服务器
	// Todo 请求数据库
	// Todo LoginGame(); // 网络请求客户端执行
}

void UGameRpc::LoginGameResult_Implementation(const ERpcErrorCode ErrorCode, const FLoginGameAck& Ack)
{
	// 将服务器发来的数据部署
	//Ack;
	if (const FOnRpcResult* Callback = *RpcCallbackPool.Find(TEXT("Login")))
	{
		Callback->Execute(ErrorCode, true);
		RpcCallbackPool.Remove(TEXT("Login"));
	}
}

void UGameRpc::K2_EnterWorld_Implementation(const int32 InRoleID)
{
	
}

void UGameRpc::EnterWorld_Implementation(const ERpcErrorCode ErrorCode, const bool bOk)
{
}

void UGameRpc::TransportPosition_Implementation(const ERpcErrorCode ErrorCode, const bool bOk)
{
}
