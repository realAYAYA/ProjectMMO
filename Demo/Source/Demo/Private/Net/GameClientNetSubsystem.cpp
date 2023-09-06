// Fill out your copyright notice in the Description page of Project Settings.


#include "GameClientNetSubsystem.h"

#include "GameRpc.h"

UGameRpc* UGameClientNetSubsystem::GetGameRpc()
{
	if (!GameRpc)
	{
		GameRpc = NewObject<UGameRpc>(this);
	}

	return GameRpc;
}
