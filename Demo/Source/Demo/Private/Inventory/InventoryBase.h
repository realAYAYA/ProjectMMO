// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryBase.generated.h"

class UItemBase;

/**
 * 
 */
UCLASS()
class DEMO_API UInventoryBase : public UObject
{
	GENERATED_BODY()
	
public:
	UInventoryBase();

	UFUNCTION(BlueprintCallable, Category = ProjectSS)
	UItemBase* GetItemByConfigID(const int32 ID) { return nullptr; };

	UFUNCTION(BlueprintCallable, Category = ProjectSS)
	UItemBase* GetItemByUID(const int32 ID) { return nullptr; };

private:

	TMap<int32, UItemBase*> AllItems;
};

UCLASS()
class DEMO_API UInventory : public UInventoryBase
{
	GENERATED_BODY()
	
public:
	UInventory() {};
	
};