// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopLevelPlayerController.generated.h"

/**
 * 用于流式关卡下的头部PlayerControler
 */
UCLASS()
class ATopLevelPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//UFUNCTION(Client, Reliable)
	//void SendToMe(const FString& In);
};
