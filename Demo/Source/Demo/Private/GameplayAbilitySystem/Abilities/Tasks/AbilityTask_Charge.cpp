// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/Tasks/AbilityTask_Charge.h"

#include "GameFramework/CharacterMovementComponent.h"

UAbilityTask_Charge* UAbilityTask_Charge::CreateChargeTask(
	UGameplayAbility* OwningAbility,
	AMCharacter* InCharacterOwner,
	const FVector InDestination)
{
	UAbilityTask_Charge* Task = NewObject<UAbilityTask_Charge>(OwningAbility);

	Task->bTickingTask = true;
	Task->Caster = InCharacterOwner;
	Task->Destination = InDestination;
	
	return Task;
}

void UAbilityTask_Charge::Activate()
{
	Super::Activate();

	BeginTime = FDateTime::Now().GetTicks();
}

void UAbilityTask_Charge::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_Charge::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (!Caster)
		return;
	
	// 冲锋超时 | 抵达位置 | 存在任何阻断移动的状态
	if (FDateTime::Now().GetTicks() - BeginTime >= 1.5 ||
		Caster->GetActorLocation().Equals(Destination) ||
		!Caster->CanMove())
	{
		OnAbilityTaskEnd.Broadcast();
		EndTask();
		return;
	}
	
	// 否则，维持冲锋状态
	const FVector DeltaDirection = (Destination - Caster->GetActorLocation()).GetSafeNormal();
	Caster->GetCharacterMovement()->Velocity += DeltaDirection * 450.0f;
	Caster->SetActorRotation(DeltaDirection.Rotation());
	
}
