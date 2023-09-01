// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/MNetworkSubsystem.h"

#include "GameRpc.h"

UGameRpc* UGameNetSubsystem::GetGameRpc()
{
	if (!GameRpc)
	{
		GameRpc = NewObject<UGameRpc>(this);
	}

	return GameRpc;
}
