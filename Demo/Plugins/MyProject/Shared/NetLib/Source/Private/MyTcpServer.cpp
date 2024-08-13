#include "MyTcpServer.h"

#include "IWebSocketNetworkingModule.h"
#include "IWebSocketServer.h"
#include "MyNetFwd.h"
#include "MyTcpConnection.h"

// ========================================================================= //

struct FTcpServerImpl
{
	TUniquePtr<IWebSocketServer> WebSocketServer;
};

// ========================================================================= //

FMyTcpServer::FMyTcpServer()
{
	ListenPort = 0;
	NextConnId = 1;
}

void FMyTcpServer::Start(const int32 ServerPort)
{
	ListenPort = ServerPort == 0 ? ListenPort : ServerPort;
	if (ListenPort <= 0)
	{
		UE_LOG(LogNetLib, Error, TEXT("%s: [TcpServer] start failed, 端口未初始化."), *FString(__FUNCTION__));
		return;
	}
	
	if (IsRunning())
	{
		UE_LOG(LogNetLib, Error, TEXT("%s: [TcpServer] start failed, 已经在运行."), *FString(__FUNCTION__));
		return;
	}

	// 回调: 产生连接时
	FWebSocketClientConnectedCallBack CallBack;
	CallBack.BindLambda([this](INetworkingWebSocket* InWebSocket)->void
	{
		auto TcpPtr = NewConnection();
		const FMySocketPtr SocketPtr = MakeShared<FMySocket>(TcpPtr->GetId());
		SocketPtr->Init(InWebSocket);
		TcpPtr->Init(SocketPtr);

		Connections.Emplace(TcpPtr->GetId(), TcpPtr);
	});
	
	ServerImpl->WebSocketServer = FModuleManager::Get().LoadModuleChecked<IWebSocketNetworkingModule>(TEXT("WebSocketNetworking")).CreateServer();
	if (!ServerImpl->WebSocketServer || !ServerImpl->WebSocketServer->Init(ServerPort, CallBack))
	{
		UE_LOG(LogNetLib, Error, TEXT("%s: [TcpServer] start failed, 创建服务器实例失败, Port=%d."), *FString(__FUNCTION__), ListenPort);
		ServerImpl->WebSocketServer.Reset();
		return;
	}

	UE_LOG(LogNetLib, Display, TEXT("%s: [TcpServer] start succssed."), *FString(__FUNCTION__));
}

void FMyTcpServer::Stop()
{
}

bool FMyTcpServer::IsRunning() const
{
	return ServerImpl->WebSocketServer.IsValid();
}

void FMyTcpServer::Tick(float DeltaTime)
{
	
}

void FMyTcpServer::RemoveConnection(const FTcpConnectionPtr& InConn)
{
	Connections.Remove(InConn->GetId());
}

uint64 FMyTcpServer::GenerateConnId()
{
	return NextConnId++;
}

FTcpConnectionPtr FPbTcpServer::NewConnection()
{
	auto Ptr = MakeShared<FPbConnection>(GenerateConnId());

	Ptr->SetPackageCallback([this](FPbConnectionPtr InConn, uint64 InCode, FMyDataBufferPtr InMessage)
	{
		
	});

	Ptr->SetConnectedCallback([this](FPbConnectionPtr InConn)
	{
		
	});

	Ptr->SetDisconnectedCallback([this](FPbConnectionPtr InConn)
	{
		
		RemoveConnection(InConn);
	});
	
	return Ptr;
}