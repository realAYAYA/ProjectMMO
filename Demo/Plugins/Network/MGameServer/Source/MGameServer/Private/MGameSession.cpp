#include "MGameSession.h"

#include "GameSessionHelper.h"

void UMGameSession::Initialize(INetworkingWebSocket* InWebSocket, const FGuid& InID)
{
	auto Ptr = TSharedPtr<INetworkingWebSocket>(InWebSocket);
	WebSocket = MoveTemp(Ptr);
	ID = InID;
	RpcInterface = MakeShared<FGameRpcInterface>(&Manager);
}

void UMGameSession::BeginDestroy()
{
	UObject::BeginDestroy();

	if (WebSocket)
	{
		WebSocket.Reset();
	}
}

void UMGameSession::Send(const FGameMessage& InMessage)
{
	Manager.SendNotify(WebSocket ,InMessage);

	LastSentTime.Store(FDateTime::UtcNow().GetTicks());
}

void UMGameSession::Send(const TArray<uint8>& Data)
{
	WebSocket->Send(Data.GetData(), Data.Num(), false);

	LastSentTime.Store(FDateTime::UtcNow().GetTicks());
}

void UMGameSession::OnConnected()
{
	FRpcServerHandleInitializers::Get().Bind(this, RpcInterface.Get());

	FWebSocketPacketReceivedCallBack ReceivedCallBack;
	ReceivedCallBack.BindUObject(this, &UMGameSession::OnReceive);
	WebSocket->SetReceiveCallBack(ReceivedCallBack);
}

void UMGameSession::OnReceive(void* InData, const int32 Size)
{
	TArray<uint8> Data;
	Data.Append((uint8*)InData, Size);

	FNetworkMessage RpcMessage;
	RpcMessage.ParseFromArray(Data);
	Manager.OnMessage(WebSocket, RpcMessage);

	LastReceivedTime.Store(FDateTime::UtcNow().GetTicks());
}
