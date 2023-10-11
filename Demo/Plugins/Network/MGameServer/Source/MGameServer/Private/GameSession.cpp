#include "GameSession.h"

FGameSession::FGameSession(INetworkingWebSocket* InWebSocket)
: WebSocket(InWebSocket), ID(FGuid::NewGuid())
{
}

void FGameSession::Send(const TArray<uint8>& Data) const
{
	WebSocket->Send(Data.GetData(), Data.Num(), false);
}

void FGameSession::OnReceive(void* InData, const int32 Size)
{
	//const TArrayView<uint8> DataArrayView = MakeArrayView(static_cast<uint8*>(InData), DataSize);
	TArray<uint8> Data;
	Data.Reserve(Size);
	memcpy(Data.GetData(), InData, Size);

	FNetworkMessage RpcMessage;
	RpcMessage.ParseFromArray(Data);
	Manager.OnMessage(WebSocket, RpcMessage);
}

