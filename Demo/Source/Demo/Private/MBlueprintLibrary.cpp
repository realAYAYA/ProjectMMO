// Fill out your copyright notice in the Description page of Project Settings.

#include "MBlueprintLibrary.h"

#include "MGameInstance.h"
#include "MGameTables/MGameTables.h"

UMGameInstance* UMBlueprintLibrary::GetMGameInstance(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World ? Cast<UMGameInstance>(World->GetGameInstance()) : nullptr;
}

UMGameTables* UMBlueprintLibrary::GetMGameTables(const UObject* WorldContextObject)
{
	if (UMGameInstance* GameInstance = GetMGameInstance(WorldContextObject))
	{
		return GameInstance->GetMGameTables();
	}
	
	return nullptr;
}

