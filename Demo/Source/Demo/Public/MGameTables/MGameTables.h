// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameTables.h"
#include "MGameTables.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMGameTables, Verbose, All);

/**
 * 
 */
UCLASS()
class DEMO_API UMGameTables : public UGameTables
{
	GENERATED_BODY()

public:

	virtual bool Init() override;

	/** 道具表 */
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	class UItemConfigManager* Item;

	/** Json常量配置 */
	//UPROPERTY(BlueprintReadOnly, Category="IdleZ")
	//FZCommonConstantsConfig CommonConstants;
};
