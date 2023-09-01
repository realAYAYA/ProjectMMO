// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Subsystems/NetworkSubsystem.h"
#include "MNetworkSubsystem.generated.h"

class UGameRpc;
/**
 * 
 */
UCLASS()
class UGameNetSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	/**  */
	UFUNCTION(BlueprintPure, Category = "ProjectM")
	UGameRpc* GetGameRpc();

private:
	
	UPROPERTY()
	UGameRpc* GameRpc;
};
