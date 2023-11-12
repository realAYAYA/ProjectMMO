// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerController.h"

#include "Demo.h"
#include "..\Public\MGameModeBase.h"
#include "MGameInstance.h"
#include "MPlayerState.h"
#include "Characters/MCharacter.h"
#include "Developer/MSaveGame.h"
#include "Net/UnrealNetwork.h"

AMPlayerController::AMPlayerController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

bool AMPlayerController::IsOnline() const
{
	const AMPlayerState* PS = GetMPlayerState();
	if (PS && PS->IsOnline())
	{
		return true;
	}

	return false;
}

AMPlayerState* AMPlayerController::GetMPlayerState() const
{
 	return Cast<AMPlayerState>(PlayerState);
}

FString AMPlayerController::K2_GetUserID() const
{
	if (const AMPlayerState* PS = GetMPlayerState())
	{
		return PS->UserID;
	}

	return TEXT("");
}

void AMPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
}
