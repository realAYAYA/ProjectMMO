// Fill out your copyright notice in the Description page of Project Settings.

#include "MBlueprintLibrary.h"


#include "MGameInstance.h"

#include "GameTablesModule.h"
#include "GameTables.h"
#include "GameMessage.h"
#include "MGameClientSubsystem.h"

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
