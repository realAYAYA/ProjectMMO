// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "GearActor.generated.h"

/**
 * 
 */
UCLASS()
class AGearActor : public AItemActor
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnEquipped(AActor* InOwner);

	UFUNCTION()
	void OnUnEquipped();

	UFUNCTION()
	virtual void OnDropped() override;
	
protected:
	virtual void InitInternal() override;

	
};
