// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameRpcClient.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MBlueprintLibrary.generated.h"

class AMPlayerState;

/**
 * 
 */
UCLASS()
class DEMO_API UMBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Test */
	UFUNCTION(BlueprintPure, Category = "ProjectM", meta = (WorldContext = "WorldContextObject"))
	static FString TestFunc();

	/** 游戏实例 */
	UFUNCTION(BlueprintPure, Category = "ProjectM", meta = (WorldContext = "WorldContextObject"))
	static class UMGameInstance* GetMGameInstance(const UObject* WorldContextObject);
	
	/** Excel配置 */
	UFUNCTION(BlueprintPure, Category = "ProjectM", meta = (WorldContext = "WorldContextObject"))
	static class UGameTables* GetMGameTables(const UObject* WorldContextObject);

	/** 客户端连接器 */
	UFUNCTION(BlueprintPure, Category = "ProjectM", meta = (WorldContext = "WorldContextObject"))
	static UGameRpcClient* GetClientRpcStub(const UObject* WorldContextObject);
};
