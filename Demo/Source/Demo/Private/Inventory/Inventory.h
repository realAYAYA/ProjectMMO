// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.generated.h"

class UItem;

/**
 * 
 */
UCLASS()
class DEMO_API UInventory : public UObject
{
	GENERATED_BODY()
	
public:
	UInventory();

	UFUNCTION(BlueprintCallable, Category = ProjectM)
	const UItem* GetItemByConfigID(const int32 ID) { return nullptr; };

	UFUNCTION(BlueprintCallable, Category = ProjectM)
	const UItem* GetItemByUID(const int32 ID) { return nullptr; };

private:

	UPROPERTY()
	TMap<int32, UItem*> AllItems;
};
