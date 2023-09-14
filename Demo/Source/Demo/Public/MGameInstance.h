// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameTypes.h"
#include "Engine/GameInstance.h"
#include "MGameInstance.generated.h"

class AMPlayerState;
class UMGameTables;

/**
 * 
 */
UCLASS()
class DEMO_API UMGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString CharacterName;

	UFUNCTION()
	UMGameTables* GetMGameTables();
	
	virtual void Init() override;

	static UMGameInstance* GetMGameInstance(const UWorld* World);
	
private:
	
	UPROPERTY()
	UMGameTables* GameTables;
};
