// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "MGameplayEffect.generated.h"

/**
 * 
 */
UCLASS()
class UMGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

	void GetSource();

	void GetTarget();
	// Todo 目标和施术者
	// Todo 持续，且影响双方的buff，例如：吸血
	// Todo 如果是持续性效果，如何取得运行时执行了第几秒（次）
};
