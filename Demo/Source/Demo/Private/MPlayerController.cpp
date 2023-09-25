// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerController.h"

#include "MGameInstance.h"
#include "MPlayerState.h"


AMPlayerController::AMPlayerController(const FObjectInitializer& ObjectInitializer)
{
}

AMPlayerState* AMPlayerController::GetMPlayerState() const
{
 	return Cast<AMPlayerState>(PlayerState);
}

void AMPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
}
