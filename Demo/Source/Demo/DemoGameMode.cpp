// Copyright Epic Games, Inc. All Rights Reserved.

#include "DemoGameMode.h"
#include "UObject/ConstructorHelpers.h"

ADemoGameMode::ADemoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Characters/BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
