// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameInstance.h"

#include "AbilitySystemGlobals.h"

void UMGameInstance::Init()
{
	Super::Init();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
