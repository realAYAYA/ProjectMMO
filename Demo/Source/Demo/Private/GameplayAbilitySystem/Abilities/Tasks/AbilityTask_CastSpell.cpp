// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/Tasks/AbilityTask_CastSpell.h"

#include "GameplayAbilitySystem/Components/MAbilitySystemComponent.h"

UAbilityTask_CastSpell* UAbilityTask_CastSpell::CreateCastSpellTask(
	UGameplayAbility* OwningAbility,
	AMCharacter* InCharacterOwner,
	const float InCastTime
	)
{
	UAbilityTask_CastSpell* Task = NewAbilityTask<UAbilityTask_CastSpell>(OwningAbility);

	Task->bTickingTask = true;
	Task->Caster = InCharacterOwner;
	Task->CastTime = InCastTime;
	
	return Task;
}

void UAbilityTask_CastSpell::Activate()
{
	Super::Activate();

	TotalTime = 0;
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
		OnAbilityCancel.Broadcast();
		EndTask();
		return;
	}
	
	const UMAbilitySystemComponent* Component = Cast<UMAbilitySystemComponent>(Caster->GetAbilitySystemComponent());
	if (!Component)
	{
		OnAbilityCancel.Broadcast();
		EndTask();
		return;
	}

	// Todo 被眩晕、被沉默、角色主动移动，取消，打断施法
	if (false)
	{
		OnAbilityCancel.Broadcast();
		EndTask();
		return;
	}

	TotalTime += DeltaTime;
	
	// 施法时间走完，施法成功
	if (TotalTime >= CastTime)
	{
		OnAbilityTaskEnd.Broadcast();
		EndTask();
	}
}
