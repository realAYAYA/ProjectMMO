#pragma once
#include "MyDataBuffer.h"
#include "MyNetFwd.h"

class INetworkingWebSocket;

//typedef TFunction<bool()>					FMySocketPrepareSendCallback;
typedef TFunction<void(FMyDataBuffer *)>	FMySocketReceivedCallback;
typedef TFunction<void()>					FMySocketClosedCallback;
typedef TFunction<void()>					FMySocketTimerCallback;

// 网络连接类实现，这是一个隔离类，是对外部网络库进行的封装
// 如果希望更换网络库，则请对该类改写
class FMySocket : public TSharedFromThis<FMySocket, ESPMode::ThreadSafe>
{
	
public:

	FMySocket(const uint64 InId);
	virtual ~FMySocket();

	void Init(INetworkingWebSocket* Socket);

	FMySocket(const FMySocket&) = delete;
	FMySocket& operator=(const FMySocket&) = delete;

	const uint64& GetId() const { return Id; }
	bool IsOpen() const { return true; }

	void Start();
	void Shutdown();

	void Send(const char* Data, uint32 Len);
	void Send(const FMyDataBufferPtr& Buffer);

	FDateTime GetLastSentTime() const { return FDateTime(LastSentTime); }
	FDateTime GetLastReceivedTime() const { return FDateTime(LastReceivedTime); }

	void SetReceivedCallback(const FMySocketReceivedCallback& Func);
	void SetClosedCallback(const FMySocketClosedCallback& Func);
	void SetTimerCallback(const FMySocketTimerCallback& Func);
	
private:

	const uint64 Id;
	std::atomic<int64> LastSentTime;// 最后发送数据时间
	std::atomic<int64> LastReceivedTime;// 最后接受数据时间
	
	TSharedPtr<INetworkingWebSocket> WebSocket = nullptr;

	FMySocketReceivedCallback ReceivedCallback;  // 接收到数据之回调 [**IO线程调用**]
	FMySocketClosedCallback ClosedCallback;  // 连接断开之回调 [**IO线程调用**]
	FMySocketTimerCallback TimerCallback;  // 连接定时器之回调 [**IO线程调用**]
};
