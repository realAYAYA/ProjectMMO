// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "MGameplayEffect.generated.h"

UENUM(BlueprintType)
enum class ERangeType : uint8
{
	None		UMETA(Displayname = "None"),	// 用于无法被驱散的效果
	Melee		UMETA(Displayname = "Melee"),
	Ranged		UMETA(Displayname = "Ranged"),
};

UENUM(BlueprintType)
enum class EBuffType : uint8
{
	None		UMETA(Displayname = "None"),	// 用于无法被驱散的效果
	Spell		UMETA(Displayname = "Spell"),
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None		UMETA(Displayname = "None"),	// 用于无法被驱散的效果
	Physical	UMETA(Displayname = "Physical"),
	Frost		UMETA(Displayname = "Frost"),
	Flame		UMETA(Displayname = "Flame"),
	Nature		UMETA(Displayname = "Nature"),
	Divine		UMETA(Displayname = "Divine"),
	Shadow		UMETA(Displayname = "Shadow"),
};

/**
 * 
 */
UCLASS()
class UMGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
	// Todo 目标和施术者
	// Todo 持续，且影响双方的buff，例如：吸血
	// Todo 如果是持续性效果，如何取得运行时执行了第几秒（次）
};
