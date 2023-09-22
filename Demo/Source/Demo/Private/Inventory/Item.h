// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS()
class UItem : public UObject
{
	GENERATED_BODY()

	UItem() {};

public:
	
	UFUNCTION(BlueprintCallable, Category = ProjectM)
	int32 GetConfigID() const {return 0;};

	UFUNCTION(BlueprintCallable, Category = ProjectM)
	FName GetConfigName() const { return TEXT("NullItem"); };

protected:
	
	int32 ConfigID = 0;

	int32 UID = 0;
};
