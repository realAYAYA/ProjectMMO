// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Subsystems/NetworkSubsystem.h"
#include "GameClientNetSubsystem.generated.h"

/**
 * Network Client Callback
 */
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnWebSocketResult, ERpcErrorCode, ErrorCode, bool, bOk);

/**
 * 
 */
UCLASS()
class UGameClientNetSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	/** 服务器单发消息给任意在线玩家 */
	//UFUNCTION(Server, Reliable)
	//void SendMessageTo(const int32 PlayerIndex);

	/** 从服务器单发消息给本机玩家 */
	//UFUNCTION(BlueprintCallable, Category = "ProjectM")
	//void SendToMe(const PlayerIndex);

private:
	
};
