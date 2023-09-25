// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerState.h"
#include "MGameInstance.h"

AMPlayerState::AMPlayerState()
{
	
}

int32 AMPlayerState::GetRoleNum() const
{
	return UserData.RoleData.Num();
}

void AMPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

void AMPlayerState::Reset()
{
	Super::Reset();
}
