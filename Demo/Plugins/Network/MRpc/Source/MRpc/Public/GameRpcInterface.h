#pragma once
#include "GameMessage.h"

class FRpcManager;

// 服务器专用 Rpc接口
class FGameRpcInterface final
{
public:
	
	explicit FGameRpcInterface(FRpcManager* InManager);
	~FGameRpcInterface();
	
	/**
	 * 登录游戏
	*/    
	typedef TFunction<void(const FLoginReq&, FLoginAck&)> FLoginGameCallback;
	FLoginGameCallback OnLoginGame;
	
};
