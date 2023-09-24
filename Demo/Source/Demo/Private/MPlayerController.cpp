// Fill out your copyright notice in the Description page of Project Settings.


#include "MPlayerController.h"

#include "MPlayerState.h"

AMPlayerController::AMPlayerController(const FObjectInitializer& ObjectInitializer)
{
	
}

void AMPlayerController::OnLogin() const
{
	OnFinishedLogin.Broadcast();
}

AMPlayerState* AMPlayerController::GetMPlayerState() const
{
 	return Cast<AMPlayerState>(PlayerState);
}

void AMPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AMPlayerState* PS = GetMPlayerState();
	if (PS)
	{
		// Todo 拿到SteamID及用户名，进行登录
		//PS->K2_Login();
		
	}

	OnPlayerStateRep.Broadcast();
	//OnFinishedLogin.Broadcast();
}
