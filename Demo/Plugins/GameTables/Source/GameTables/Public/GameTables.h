// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameTables.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGameTables, Verbose, All);

/*
 * 游戏表格配置
 */
UCLASS(BlueprintType, Blueprintable)
class GAMETABLES_API UGameTables : public UObject
{
	GENERATED_BODY()

public:
	
	virtual bool Init();

	/** 道具表 */
	//UPROPERTY(BlueprintReadOnly, Category="IdleZ")
	//UZItemConfigManager* Item;
	
	/** Json常量配置 */
	//UPROPERTY(BlueprintReadOnly, Category="IdleZ")
	//FZCommonConstantsConfig CommonConstants;
	
};
