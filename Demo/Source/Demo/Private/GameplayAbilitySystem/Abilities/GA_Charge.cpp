// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Charge.h"

#include "VectorUtil.h"
#include "Characters/MCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tasks/AbilityTask_Charge.h"

UGA_Charge::UGA_Charge()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	TargetType = ETargetType::Hostile;
	Range = 2000;
	MinRange = 500;
}

void UGA_Charge::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	AMCharacter* Caster = Cast<AMCharacter>(ActorInfo->AvatarActor);
}

void UGA_Charge::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
}

bool UGA_Charge::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const AMCharacter* Caster = Cast<AMCharacter>(ActorInfo->AvatarActor);
	if (!Caster || !Caster->GetAbilitySystemComponent())
		return false;

	const AMCharacter* Target = Caster->GetCurrentTarget();
	if (!Target || !Target->GetAbilitySystemComponent())
	{
		return false;
	}

	// Todo 不是敌对目标
	
	// Out of range
	const float Distance = (Caster->GetActorLocation() - Target->GetActorLocation()).Length();
	if (Distance > Range || Distance < MinRange)
		return false;

	// 没有朝向敌人
	const FVector Dir = UKismetMathLibrary::Normal(Target->GetActorLocation() - Caster->GetActorLocation(), 0.0001);
	if (UKismetMathLibrary::Dot_VectorVector(Dir, Caster->GetActorForwardVector()) < 0.5f)
		return false;
	
	// Todo 不在视野中
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_Charge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 根据目标位置，计算冲锋的目的地
	AMCharacter* Caster = Cast<AMCharacter>(OwnerInfo->AvatarActor);
	const AMCharacter* Target = Cast<AMCharacter>(OwnerInfo->AvatarActor)->GetCurrentTarget();
	
	FVector Destination = Target->GetActorLocation();
	const float Radius = Target->GetCapsuleComponent()->GetScaledCapsuleRadius() + 5.0f;
	const FVector Dir = UKismetMathLibrary::Normal(Target->GetActorLocation() - Caster->GetActorLocation(), 0.0001);
	Destination = Destination - Dir * Radius;
	
	ChargeTask = UAbilityTask_Charge::CreateChargeTask(this, Caster, Destination);
	
	ChargeTask->OnAbilityTaskEnd.AddDynamic(this, &UGA_Charge::K2_EndAbility);
	ChargeTask->OnAbilityCancel.AddDynamic(this, &UGA_Charge::K2_CancelAbility);
	ChargeTask->ReadyForActivation();// 启动任务
	
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
}

void UGA_Charge::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ChargeTask)
		ChargeTask->EndTask();
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_Charge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ChargeTask)
		ChargeTask->EndTask();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

