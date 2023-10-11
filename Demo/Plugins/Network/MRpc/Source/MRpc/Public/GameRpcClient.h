#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "NetFwd.h"
#include "GameMessage.h"

#include "GameRpcClient.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnLoginResult, ERpcErrorCode, InErrorCode, FLoginAck, InData);

class FRpcManager;

UCLASS(BlueprintType, Blueprintable)
class UGameRpcClient : public UObject
{
	GENERATED_BODY()
public:

	void Setup(FRpcManager* InManager, const FConnectionPtr& InConn);

	/**
	 * 登录游戏
	*/
	UFUNCTION(BlueprintCallable, Category="IdleZ", DisplayName="LoginGame")
	void K2_LoginGame(const FLoginReq& InParams, const FOnLoginResult& InCallback);
    
	typedef TFunction<void(ERpcErrorCode, const FLoginAck&)> OnLoginGameResult;
	void LoginGame(const FLoginReq& InReqMessage, const OnLoginGameResult& InCallback);

private:

	FRpcManager* Manager = nullptr;

	FConnectionPtr Connection;
};
