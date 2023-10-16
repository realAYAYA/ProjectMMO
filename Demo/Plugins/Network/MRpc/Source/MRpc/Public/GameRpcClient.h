#pragma once
#include "CoreMinimal.h"

#include "NetFwd.h"
#include "GameMessage.h"


#include "GameRpcClient.generated.h"

class FClientRpcManager;


DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnLoginGameResult, ERpcErrorCode, InErrorCode, FLoginGameAck, InData);




UCLASS(BlueprintType, Blueprintable)
class MRPC_API UGameRpcClient : public UObject
{
    GENERATED_BODY()

public:

    void Setup(FClientRpcManager* InManager, const FConnectionPtr& InConn);

    /**
     * 登录游戏
    */
    UFUNCTION(BlueprintCallable, Category = "PorjectM", DisplayName="LoginGame")
    void K2_LoginGame(const FLoginGameReq& InParams, const FOnLoginGameResult& InCallback);
    
    typedef TFunction<void(ERpcErrorCode, const FLoginGameAck&)> FOnLoginGameResultFunction;
    void LoginGame(const FLoginGameReq& InReqMessage, const FOnLoginGameResultFunction& InCallback) const;

    
     
private:

    FClientRpcManager* Manager = nullptr;

    FConnectionPtr Connection;
};
