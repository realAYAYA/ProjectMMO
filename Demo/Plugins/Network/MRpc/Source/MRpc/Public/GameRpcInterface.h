#pragma once
#include "MGameMessage.h"


class FServerRpcManager;

class MRPC_API FGameRpcInterface
{

public:

    const TCHAR* GetName() const { return TEXT("GameRpc"); }

    explicit FGameRpcInterface(FServerRpcManager* InManager);

    virtual ~FGameRpcInterface() {}
    
    
    /**
     * DS登录WebSocket服务器
    */
    typedef TFunction<void(const FLoginAsDSReq&, FLoginAsDSAck&)> FLoginAsDSCallback;
    FLoginAsDSCallback OnLoginAsDS;
    
    /**
     * DS拉取玩家数据
    */
    typedef TFunction<void(const FPullRoleDataReq&, FPullRoleDataAck&)> FPullRoleDataCallback;
    FPullRoleDataCallback OnPullRoleData;
    
    /**
     * DS提交玩家数据
    */
    typedef TFunction<void(const FCommitRoleDataReq&, FCommitRoleDataAck&)> FCommitRoleDataCallback;
    FCommitRoleDataCallback OnCommitRoleData;
    
    /**
     * 登录游戏
    */
    typedef TFunction<void(const FLoginGameReq&, FLoginGameAck&)> FLoginGameCallback;
    FLoginGameCallback OnLoginGame;
    
    /**
     * 登录游戏
    */
    typedef TFunction<void(const FLogoutGameReq&, FLogoutGameAck&)> FLogoutGameCallback;
    FLogoutGameCallback OnLogoutGame;
    
    /**
     * 创建角色
    */
    typedef TFunction<void(const FCreateRoleReq&, FCreateRoleAck&)> FCreateRoleCallback;
    FCreateRoleCallback OnCreateRole;
    
    /**
     * 进入世界
    */
    typedef TFunction<void(const FEnterWorldReq&, FEnterWorldAck&)> FEnterWorldCallback;
    FEnterWorldCallback OnEnterWorld;
    

};
