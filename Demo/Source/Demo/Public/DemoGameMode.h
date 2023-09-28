// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemoGameMode.generated.h"

class AMPlayerState;

UCLASS(minimalapi)
class AMGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	AMGameMode();

	UFUNCTION(BlueprintCallable, Category = "ProjecetM")
	const AMPlayerState* FindOnlinePlayerByID(const FString InID) const;

	UFUNCTION(BlueprintCallable, Category = "ProjecetM")
	const AMPlayerState* FindOnlinePlayerByName(const FString& InName) const;

	UFUNCTION(BlueprintCallable, Category = "ProjecetM")
	TArray<AMPlayerState*> GetAllPlayerStates() const;
};
