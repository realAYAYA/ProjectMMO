// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerState.h"

AMPlayerState::AMPlayerState()
{
}

void AMPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

void AMPlayerState::Reset()
{
	Super::Reset();
}
