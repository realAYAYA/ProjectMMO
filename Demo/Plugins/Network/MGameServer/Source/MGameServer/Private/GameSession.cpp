#include "GameSession.h"

#include "GameSessionHelper.h"

FGameSession::FGameSession(INetworkingWebSocket* InWebSocket, const FGuid& InID)
: WebSocket(InWebSocket), ID(FGuid::NewGuid())
{
	RpcInterface = MakeShared<FGameRpcInterface>(&Manager);
}

FGameSession::FGameSession(FGameSession&& Right) noexcept
: WebSocket(Right.WebSocket), ID(Right.ID)
{
	RpcInterface = MakeShared<FGameRpcInterface>(&Manager);
}

void FGameSession::Send(const TArray<uint8>& Data) const
{
	WebSocket->Send(Data.GetData(), Data.Num(), false);
}

void FGameSession::OnConnected()
{
	FRpcServerHandleInitializers::Get().Bind(this, RpcInterface.Get());
}

void FGameSession::OnReceive(void* InData, const int32 Size)
{
	//const TArrayView<uint8> DataArrayView = MakeArrayView(static_cast<uint8*>(InData), DataSize);
	TArray<uint8> Data;
	Data.Append((uint8*)InData, Size);

	FNetworkMessage RpcMessage;
	RpcMessage.ParseFromArray(Data);
	Manager.OnMessage(WebSocket, RpcMessage);
}
