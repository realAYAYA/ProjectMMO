// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "MGameTypes.h"

#include "MPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedLogin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerStateRep);

class AMPlayerState;
/**
 * 
 */
UCLASS()
class DEMO_API AMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Todo 登录完成调用*/
	UFUNCTION(BlueprintCallable, Category = "ProjecetM")
	void OnLogin() const;
	
	/** 不保证在游戏开始（网络同步完成前）时，可以拿到非空的PlayerState*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ProjecetM")
	AMPlayerState* GetMPlayerState() const;

	UPROPERTY(BlueprintAssignable)
	FOnFinishedLogin OnFinishedLogin;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateRep OnPlayerStateRep;

protected:

	virtual void OnRep_PlayerState() override;
};
