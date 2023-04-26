// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class UItemBase : public UObject
{
	GENERATED_BODY()

	UItemBase() {};

public:
	UFUNCTION(BlueprintCallable, Category = ProjectSS)
	int32 GetConfigID() const {return 0;};

	UFUNCTION(BlueprintCallable, Category = ProjectSS)
	FName GetConfigName() const { return TEXT("NullItem"); };

protected:
	int32 ConfigID = 0;

	int32 UID = 0;
};
