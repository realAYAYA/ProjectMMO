// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameTypes.h"
#include "RpcObject.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API URpcObject : public UObject
{
	GENERATED_BODY()
	
public:
	
	URpcObject(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	
	UPROPERTY()
	TMap<FString, FOnRpcResult> RpcManager;
};
