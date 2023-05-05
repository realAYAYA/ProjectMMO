// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataTable.h"
#include "GameTables.generated.h"

/*
 * 游戏表格配置
 */
UCLASS(BlueprintType, Blueprintable)
class GAMETABLES_API UGameTables : public UObject
{
	GENERATED_BODY()

public:
	bool Init();

	/** Json常量配置 */
	//UPROPERTY(BlueprintReadOnly, Category="IdleZ")
	//FZCommonConstantsConfig CommonConstants;
	
};
