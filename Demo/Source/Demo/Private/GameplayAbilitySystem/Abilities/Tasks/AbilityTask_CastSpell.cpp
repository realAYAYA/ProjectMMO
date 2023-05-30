// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/Tasks/AbilityTask_CastSpell.h"

#include "GameplayAbilitySystem/Components/MAbilitySystemComponent.h"

UAbilityTask_CastSpell* UAbilityTask_CastSpell::CreateChargeTask(
	UGameplayAbility* OwningAbility,
	AMCharacter* InCharacterOwner)
{
	UAbilityTask_CastSpell* Task = NewObject<UAbilityTask_CastSpell>(OwningAbility);

	Task->bTickingTask = true;
	Task->Caster = InCharacterOwner;
	
	return Task;
}

void UAbilityTask_CastSpell::Activate()
{
	Super::Activate();

	BeginTime = FDateTime::Now().GetTicks();
}

void UAbilityTask_CastSpell::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_CastSpell::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (!Caster)
	{
		EndTask();
		return;
	}
	
	const UMAbilitySystemComponent* Component = Cast<UMAbilitySystemComponent>(Caster->GetAbilitySystemComponent());
	if (!Component)
	{
		EndTask();
		return;
	}

	// Todo 被眩晕、被沉默
}
