// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameInstance.h"

#include "AbilitySystemGlobals.h"

#include "MGameTables/MGameTables.h"

void UMGameInstance::Init()
{
	Super::Init();

	UAbilitySystemGlobals::Get().InitGlobalData();

	GetMGameTables();
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
