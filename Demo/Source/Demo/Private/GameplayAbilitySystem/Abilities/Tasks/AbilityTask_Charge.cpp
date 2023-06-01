// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/Tasks/AbilityTask_Charge.h"

#include "Components/CapsuleComponent.h"
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
	
	/*const UMAbilitySystemComponent* Component = Cast<UMAbilitySystemComponent>(Caster->GetAbilitySystemComponent());
	if (!Component && !Component->CanMove() || FDateTime::Now().GetTicks() - BeginTime >= 1.3)
	{
		OnAbilityCancel.Broadcast();
		EndTask();
		return;
	}*/
	
	// 冲锋超时 | 抵达位置 | 存在任何阻断移动的状态
	if ((Caster->GetActorLocation() - Destination).Length() - Caster->GetCapsuleComponent()->GetScaledCapsuleRadius() <= 50.0f)
	{
		OnAbilityTaskEnd.Broadcast();
		EndTask();
		return;
	}
	
	// 否则，维持冲锋状态
	const FVector Dir = (Destination - Caster->GetActorLocation()).GetSafeNormal();
	Caster->SetActorRotation(Dir.Rotation());
	Caster->AddMovementInput(Caster->GetActorForwardVector(), 1.0f);
	
}
