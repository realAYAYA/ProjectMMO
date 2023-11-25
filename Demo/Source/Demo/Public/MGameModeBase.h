// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MGameModeBase.generated.h"

class AMPlayerState;

UCLASS(minimalapi)
class AMGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	AMGameMode();
	
	const AMPlayerState* FindOnlinePlayerByID(const uint64 InID) const;

	UFUNCTION(BlueprintCallable, Category = "ProjecetM")
	const AMPlayerState* FindOnlinePlayerByName(const FString& InName) const;

	UFUNCTION(BlueprintCallable, Category = "ProjecetM")
	TArray<AMPlayerState*> GetAllPlayerStates() const;
	
	virtual void BeginPlay() override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
};
