// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MBlueprintLibrary.generated.h"

class AMPlayerState;
class UGameRpc;

/**
 * 
 */
UCLASS()
class DEMO_API UMBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** 游戏实例 */
	UFUNCTION(BlueprintPure, Category = "ProjectM", meta = (WorldContext = "WorldContextObject"))
	static class UMGameInstance* GetMGameInstance(const UObject* WorldContextObject);

	/** Client网络接口 */
	UFUNCTION(BlueprintPure, Category = "ProjectM", meta = (WorldContext = "WorldContextObject"))
	static const UGameRpc* GetGameRpc(const UObject* WorldContextObject);

	/** 主角数据 */
	UFUNCTION(BlueprintPure, Category = "ProjectM", meta = (WorldContext = "WorldContextObject"))
	static AMPlayerState* GetMPlayerState(const UObject* WorldContextObject);
	
	/** Excel配置 */
	UFUNCTION(BlueprintPure, Category = "ProjectM", meta = (WorldContext = "WorldContextObject"))
	static class UMGameTables* GetMGameTables(const UObject* WorldContextObject);
	
};
