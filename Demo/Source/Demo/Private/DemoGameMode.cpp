// Copyright Epic Games, Inc. All Rights Reserved.

#include "DemoGameMode.h"

#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameStateBase.h"

#include "MPlayerState.h"

AMGameMode::AMGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Characters/BP_MCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

const AMPlayerState* AMGameMode::FindOnlinePlayerByID(const FString InID) const
{
	if (!GameState)
		return nullptr;
	
	for (const auto PlayerState : GameState.Get()->PlayerArray)
	{
		const AMPlayerState* MPlayerState = Cast<AMPlayerState>(PlayerState.Get());
		if (MPlayerState && MPlayerState->UserID == InID)
			return MPlayerState;
	}

	return nullptr;
}

const AMPlayerState* AMGameMode::FindOnlinePlayerByName(const FString& InName) const
{
	if (!GameState)
		return nullptr;
	
	for (const auto PlayerState : GameState.Get()->PlayerArray)
	{
		const AMPlayerState* MPlayerState = Cast<AMPlayerState>(PlayerState.Get());
		if (MPlayerState && MPlayerState->GetPlayerName() == InName)
			return MPlayerState;
	}

	return nullptr;
}

TArray<AMPlayerState*> AMGameMode::GetAllPlayerStates() const
{
	TArray<AMPlayerState*> Array;
	for (const auto PlayerState : GameState.Get()->PlayerArray)
	{
		if (AMPlayerState* MPlayerState = Cast<AMPlayerState>(PlayerState.Get()))
		{
			Array.Add(MPlayerState);
		}
	}

	return Array;
}
