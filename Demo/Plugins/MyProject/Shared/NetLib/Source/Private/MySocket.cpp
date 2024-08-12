#include "MySocket.h"
#include "INetworkingWebSocket.h"
#include "MTools.h"

FMySocket::FMySocket(const uint64 InId): Id(InId)
{
}

FMySocket::~FMySocket()
{
}

void FMySocket::Init(INetworkingWebSocket* Socket)
{
	auto Ptr = TSharedPtr<INetworkingWebSocket>(Socket);
	WebSocket = MoveTemp(Ptr);
}

void FMySocket::Start()
{
	
}

void FMySocket::Shutdown()
{
	if (WebSocket)
		WebSocket.Reset();
}

void FMySocket::Send(const char* Data, uint32 Len)
{
}

void FMySocket::Send(const FMyDataBufferPtr& Buffer)
{
	WebSocket->Send(reinterpret_cast<uint8*>(const_cast<char*>(Buffer->Peek())), Buffer->ReadableBytes() * sizeof(char));

	LastSentTime = FMyTools::Now().GetTicks();
}

void FMySocket::SetReceivedCallback(const FMySocketReceivedCallback& Func)
{
	ReceivedCallback = Func;
}

void FMySocket::SetClosedCallback(const FMySocketClosedCallback& Func)
{
	ClosedCallback = Func;
}

void FMySocket::SetTimerCallback(const FMySocketTimerCallback& Func)
{
	TimerCallback = Func;
}

