// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/Tasks/AbilityTask_Charge.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayAbilitySystem/Components/MAbilitySystemComponent.h"

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
	{
		OnAbilityCancel.Broadcast();
		EndTask();
		return;
	}
	
	const UMAbilitySystemComponent* Component = Cast<UMAbilitySystemComponent>(Caster->GetAbilitySystemComponent());
	if (!Component && !Component->CanMove() || FDateTime::Now().GetTicks() - BeginTime >= 1.3)
	{
		OnAbilityCancel.Broadcast();
		EndTask();
		return;
	}
	
	// 冲锋超时 | 抵达位置 | 存在任何阻断移动的状态
	if (Caster->GetActorLocation().Equals(Destination))
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
