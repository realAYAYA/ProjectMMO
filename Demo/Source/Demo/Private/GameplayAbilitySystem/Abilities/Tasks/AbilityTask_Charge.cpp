// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/Tasks/AbilityTask_Charge.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayAbilitySystem/AttributeSets/MAttributeSet.h"

#include "GameplayAbilitySystem/Components/MAbilitySystemComponent.h"

UAbilityTask_Charge* UAbilityTask_Charge::CreateChargeTask(
	UGameplayAbility* OwningAbility,
	AMCharacter* InCharacterOwner,
	const FVector InDestination)
{
	UAbilityTask_Charge* Task = NewAbilityTask<UAbilityTask_Charge>(OwningAbility);

	Task->bTickingTask = true;
	Task->Caster = InCharacterOwner;
	Task->Destination = InDestination;
	
	return Task;
}

void UAbilityTask_Charge::Activate()
{
	Super::Activate();

	CastTime = 0;
	Caster->GetCharacterMovement()->MaxAcceleration = 1500000;
}

void UAbilityTask_Charge::OnDestroy(bool bInOwnerFinished)
{
	Caster->GetCharacterMovement()->MaxAcceleration = 1500;
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_Charge::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (!Caster)
	{
		Caster->GetCharacterMovement()->MaxAcceleration = 1500;
		OnAbilityCancel.Broadcast();
		EndTask();
		return;
	}

	CastTime += DeltaTime;
	
	// 抵达位置 | 冲锋超时
	if ((Caster->GetActorLocation() - Destination).Length() - Caster->GetCapsuleComponent()->GetScaledCapsuleRadius() <= 60.0f
		|| CastTime >= 3)
	{
		Caster->GetCharacterMovement()->MaxAcceleration = 1500;
		OnAbilityTaskEnd.Broadcast();
		EndTask();
		return;
	}
	
	// 否则，维持冲锋状态
	const FVector Dir = (Destination - Caster->GetActorLocation()).GetSafeNormal();
	Caster->SetActorRotation(Dir.Rotation());
	Caster->AddMovementInput(Caster->GetActorForwardVector(), 1.0f);
}
