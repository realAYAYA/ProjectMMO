// Fill out your copyright notice in the Description page of Project Settings.

#include "MBlueprintLibrary.h"


#include "MGameInstance.h"

#include "GameTablesModule.h"
#include "GameTables.h"
#include "MGameMessage.h"
#include "Characters/MCharacter.h"

FString UMBlueprintLibrary::TestFunc()
{
	const FLoginGameReq A;

	FString Result = TEXT("---");

	return Result;
}

UMGameInstance* UMBlueprintLibrary::GetMGameInstance(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World ? Cast<UMGameInstance>(World->GetGameInstance()) : nullptr;
}

UGameTables* UMBlueprintLibrary::GetMGameTables(const UObject* WorldContextObject)
{
	if (const UMGameInstance* GameInstance = GetMGameInstance(WorldContextObject))
	{
		return FGameTablesModule::Get().GetGameTables();
	}
	
	return nullptr;
}

UGameRpcClient* UMBlueprintLibrary::GetClientRpcStub(const UObject* WorldContextObject)
{
	if (const UMGameInstance* GameInstance = GetMGameInstance(WorldContextObject))
	{
		return GameInstance->GetClientRpcStub();
	}
	
	return nullptr;
}

int64 UMBlueprintLibrary::GetPlayerID(const UObject* WorldContextObject)
{
	if (const UMGameInstance* GameInstance = GetMGameInstance(WorldContextObject))
	{
		return GameInstance->GetPlayerID();
	}
	
	return 0;
}

bool UMBlueprintLibrary::IsOnline(const UObject* WorldContextObject)
{
	if (const UMGameInstance* GameInstance = GetMGameInstance(WorldContextObject))
	{
		return GameInstance->GetClientRpcStub() != nullptr && GameInstance->GetPlayerID() != 0;
	}
	
	return false;
}

FString UMBlueprintLibrary::GetNetAddress()
{
	if (IsRunningDedicatedServer())
	{
		
	}
	else
	{
		
		
	}

	return TEXT("Not Running as DedicatedServer!");
}

bool UMBlueprintLibrary::IsFriendly(const AMCharacter* A, const AMCharacter* B)
{
	if (A->GetRoleCamp() == B->GetRoleCamp())
		return true;
	
	return false;
}
