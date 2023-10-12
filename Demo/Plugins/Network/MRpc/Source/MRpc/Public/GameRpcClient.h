#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "NetFwd.h"
#include "GameMessage.h"

#include "GameRpcClient.generated.h"

class FClientRpcManager;

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnLoginResult, const ERpcErrorCode, InErrorCode, const FLoginAck&, InData);


UCLASS(BlueprintType, Blueprintable)
class MRPC_API UGameRpcClient : public UObject
{
	GENERATED_BODY()
public:

	void Setup(FClientRpcManager* InManager, const FConnectionPtr& InConn);

	/**
	 * 登录游戏
	*/
	UFUNCTION(BlueprintCallable, Category="IdleZ", DisplayName="LoginGame")
	void K2_LoginGame(const FLoginReq& InParams, const FOnLoginResult& InCallback) const;
    
	typedef TFunction<void(ERpcErrorCode, const FLoginAck&)> FOnLoginGameResultFunction;
	void LoginGame(const FLoginReq& InReqMessage, const FOnLoginGameResultFunction& InCallback) const;

private:

	FClientRpcManager* Manager = nullptr;

	FConnectionPtr Connection;
};