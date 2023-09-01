// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameInstance.h"

#include "AbilitySystemGlobals.h"

#include "MGameTables/MGameTables.h"
#include "Net/MNetworkSubsystem.h"

void UMGameInstance::Init()
{
	Super::Init();

	UAbilitySystemGlobals::Get().InitGlobalData();

	GetMGameTables();
}

UMGameInstance* UMGameInstance::GetMGameInstance(const UWorld* World)
{
	return World ? Cast<UMGameInstance>(World->GetGameInstance()) : nullptr;
}

AMPlayerState* UMGameInstance::GetMPlayerState(const UWorld* World)
{
	if (const UMGameInstance* GameInstance = GetMGameInstance(World))
	{
		return GameInstance->Player;
	}
	
	return nullptr;
}

const UGameRpc* UMGameInstance::GetGameRpc() const
{
	return GetSubsystem<UGameNetSubsystem>()->GetGameRpc();
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
