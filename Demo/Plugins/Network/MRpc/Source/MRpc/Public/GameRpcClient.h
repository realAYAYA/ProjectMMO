#pragma once
#include "CoreMinimal.h"

#include "NetFwd.h"
#include "GameMessage.h"


#include "GameRpcClient.generated.h"

class FClientRpcManager;


DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnLoginAsDSResult, ERpcErrorCode, InErrorCode, FLoginAsDSAck, InData);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnPullRoleDataResult, ERpcErrorCode, InErrorCode, FPullRoleDataAck, InData);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnCommitRoleDataResult, ERpcErrorCode, InErrorCode, FCommitRoleDataAck, InData);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnLoginGameResult, ERpcErrorCode, InErrorCode, FLoginGameAck, InData);



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateChatMessageResult, FUpdateChatMessage, InData);


UCLASS(BlueprintType, Blueprintable)
class MRPC_API UGameRpcClient : public UObject
{
    GENERATED_BODY()

public:

    void Setup(FClientRpcManager* InManager, const FConnectionPtr& InConn);

    /**
     * DS登录WebSocket服务器
    */
    UFUNCTION(BlueprintCallable, Category = "PorjectM", DisplayName="LoginAsDS")
    void K2_LoginAsDS(const FLoginAsDSReq& InParams, const FOnLoginAsDSResult& InCallback);
    
    typedef TFunction<void(ERpcErrorCode, const FLoginAsDSAck&)> FOnLoginAsDSResultFunction;
    void LoginAsDS(const FLoginAsDSReq& InReqMessage, const FOnLoginAsDSResultFunction& InCallback) const;

    /**
     * DS拉取玩家数据
    */
    UFUNCTION(BlueprintCallable, Category = "PorjectM", DisplayName="PullRoleData")
    void K2_PullRoleData(const FPullRoleDataReq& InParams, const FOnPullRoleDataResult& InCallback);
    
    typedef TFunction<void(ERpcErrorCode, const FPullRoleDataAck&)> FOnPullRoleDataResultFunction;
    void PullRoleData(const FPullRoleDataReq& InReqMessage, const FOnPullRoleDataResultFunction& InCallback) const;

    /**
     * DS提交玩家数据
    */
    UFUNCTION(BlueprintCallable, Category = "PorjectM", DisplayName="CommitRoleData")
    void K2_CommitRoleData(const FCommitRoleDataReq& InParams, const FOnCommitRoleDataResult& InCallback);
    
    typedef TFunction<void(ERpcErrorCode, const FCommitRoleDataAck&)> FOnCommitRoleDataResultFunction;
    void CommitRoleData(const FCommitRoleDataReq& InReqMessage, const FOnCommitRoleDataResultFunction& InCallback) const;

    /**
     * 登录游戏
    */
    UFUNCTION(BlueprintCallable, Category = "PorjectM", DisplayName="LoginGame")
    void K2_LoginGame(const FLoginGameReq& InParams, const FOnLoginGameResult& InCallback);
    
    typedef TFunction<void(ERpcErrorCode, const FLoginGameAck&)> FOnLoginGameResultFunction;
    void LoginGame(const FLoginGameReq& InReqMessage, const FOnLoginGameResultFunction& InCallback) const;

    
    /**
     * 收到聊天消息通知
    */
    UPROPERTY(BlueprintAssignable, Category = "ProjectM") 
    FOnUpdateChatMessageResult OnUpdateChatMessage;
    
     
private:

    FClientRpcManager* Manager = nullptr;

    FConnectionPtr Connection;
};
