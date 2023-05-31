// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Charge.h"

#include "Characters/MCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Tasks/AbilityTask_Charge.h"

UMGA_Charge::UMGA_Charge()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMGA_Charge::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	AMCharacter* Caster = Cast<AMCharacter>(ActorInfo->AvatarActor);
}

void UMGA_Charge::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
}

bool UMGA_Charge::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const AMCharacter* Caster = Cast<AMCharacter>(ActorInfo->AvatarActor);
	if (!Caster)
		return false;

	const AMCharacter* Target = Caster->GetCurrentTarget();
	if (!Target)
		return false;// Todo 敌对阵营

	if ((Caster->GetActorLocation() - Target->GetActorLocation()).Length() > 80)
		return false;
	// Todo 不在视野中
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UMGA_Charge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 根据目标位置，计算冲锋的目的地
	AMCharacter* Caster = Cast<AMCharacter>(OwnerInfo->AvatarActor);
	AMCharacter* Target = Cast<AMCharacter>(OwnerInfo->AvatarActor)->GetCurrentTarget();
	
	FVector Destination = Target->GetActorLocation();
	const float Radius = Target->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector Dir = (Target->GetActorLocation() - Caster->GetActorLocation()).GetSafeNormal();
	Destination = Destination - Dir * Radius;
	
	ChargeTask = UAbilityTask_Charge::CreateChargeTask(this, Caster, Destination);
	ChargeTask->OnAbilityTaskEnd.AddDynamic(this, &UMGA_Charge::K2_EndAbility);
	ChargeTask->OnAbilityCancel.AddDynamic(this, &UMGA_Charge::K2_CancelAbility);
	ChargeTask->ReadyForActivation();// 启动任务
	
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
}

void UMGA_Charge::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMGA_Charge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

