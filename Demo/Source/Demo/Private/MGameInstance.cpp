// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameInstance.h"

#include "AbilitySystemGlobals.h"
#include "Demo.h"
#include "Developer/MSaveGame.h"
#include "Kismet/GameplayStatics.h"

#include "MGameTables/MGameTables.h"
#include "Net\GameClientNetSubsystem.h"

void UMGameInstance::Init()
{
	Super::Init();

	UAbilitySystemGlobals::Get().InitGlobalData();

	GetMGameTables();
	
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("MSaveGame"), 1))
	{
		SaveGame = NewObject<UMSaveGame>(this);
	}
	else
	{
		SaveGame = Cast<UMSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MSaveGame"), 1));
		if (!SaveGame)
			SaveGame = NewObject<UMSaveGame>(this);
	}
}

void UMGameInstance::BeginDestroy()
{
	if (UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("MSaveGame"), 1))
		UE_LOG(LogProjectM, Error, TEXT("SaveGame Failed."));
	
	Super::BeginDestroy();
}

UMGameInstance* UMGameInstance::GetMGameInstance(const UWorld* World)
{
	return World ? Cast<UMGameInstance>(World->GetGameInstance()) : nullptr;
}

UMGameTables* UMGameInstance::GetMGameTables()
{
	if (!GameTables)
	{
		UMGameTables* NewOne = NewObject<UMGameTables>();
		NewOne->AddToRoot();
		NewOne->Init();
		GameTables = NewOne;
	}
	
	return GameTables;
}
