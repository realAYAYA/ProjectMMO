// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/Tasks/AbilityTask_Charge.h"

UAbilityTask_Charge* UAbilityTask_Charge::CreateChargeTask(UGameplayAbility* OwningAbility,
	AMCharacter* InCharacterOwner, FVector InDestination)
{
	
}

void UAbilityTask_Charge::Activate()
{
	Super::Activate();
}

void UAbilityTask_Charge::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_Charge::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
}
