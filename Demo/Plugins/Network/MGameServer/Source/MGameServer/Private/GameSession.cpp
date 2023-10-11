#include "GameSession.h"

FGameSession::FGameSession(INetworkingWebSocket* InWebSocket)
: WebSocket(InWebSocket), ID(FGuid::NewGuid())
{
	FWebSocketPacketReceivedCallBack ReceivedCallBack;
	ReceivedCallBack.BindUObject(this, &FGameSession::OnReceive);
	InWebSocket->SetReceiveCallBack(ReceivedCallBack);
}

void FGameSession::Send(const TArray<uint8>& Data) const
{
	WebSocket->Send(Data.GetData(), Data.Num(), false);
}

void FGameSession::OnReceive(void* InData, const int32 DataSize)
{
	const TArrayView<uint8> DataArrayView = MakeArrayView(static_cast<uint8*>(InData), DataSize);
	
}

