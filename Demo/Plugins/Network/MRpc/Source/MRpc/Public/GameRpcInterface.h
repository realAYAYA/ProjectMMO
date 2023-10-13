#pragma once
#include "GameMessage.h"

class FServerRpcManager;

// 服务器专用 Rpc接口
class FGameRpcInterface final
{
	
public:
	
	const TCHAR* GetName() const { return TEXT("GameRpc"); }
	
	explicit FGameRpcInterface(FServerRpcManager* InManager);
	~FGameRpcInterface();
	
	/**
	 * 登录游戏
	*/    
	typedef TFunction<void(const FLoginGameReq&, FLoginGameAck&)> FLoginGameCallback;
	FLoginGameCallback OnLoginGame;
	
};
