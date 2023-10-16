#pragma once
#include "GameMessage.h"


class FServerRpcManager;

class MRPC_API FGameRpcInterface
{

public:

    const TCHAR* GetName() const { return TEXT("GameRpc"); }

    explicit FGameRpcInterface(FServerRpcManager* InManager);

    virtual ~FGameRpcInterface() {}
    
    
    /**
     * 登录游戏
    */
    typedef TFunction<void(const FLoginGameReq&, FLoginGameAck&)> FLoginGameCallback;
    FLoginGameCallback OnLoginGame;
    

};
