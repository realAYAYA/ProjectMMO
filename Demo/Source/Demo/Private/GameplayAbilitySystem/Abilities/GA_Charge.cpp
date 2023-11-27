// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Charge.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tasks/AbilityTask_Charge.h"

#include "Characters/MCharacter.h"
#include "GameplayAbilitySystem/GameplayEffects/MGameplayEffect.h"

UGA_Charge::UGA_Charge()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	Range = 2000;
	MinRange = 500;
}

EActivateFailCode UGA_Charge::CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const
{
	const AMCharacter* Caster = Cast<AMCharacter>(ActorInfo.AvatarActor.Get());
	
	EActivateFailCode FailCode = Super::CanActivateCondition(ActorInfo);
	if (FailCode != EActivateFailCode::Success)
	{
		Caster->OnAbilityFailed.Broadcast(FailCode);
		return FailCode;
	}

	// Todo 路径检查

	Caster->OnAbilityFailed.Broadcast(FailCode);

	return FailCode;
}

void UGA_Charge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
	
	AMCharacter* Caster = Cast<AMCharacter>(OwnerInfo->AvatarActor);
	// Target已经在父类虚函数中指定
	
	// 根据目标位置，计算冲锋的目的地
	FVector Destination = Target->GetActorLocation();
	const float Radius = Target->GetCapsuleComponent()->GetScaledCapsuleRadius() + 5.0f;
	const FVector Dir = UKismetMathLibrary::Normal(Target->GetActorLocation() - Caster->GetActorLocation(), 0.0001);
	Destination = Destination - Dir * Radius;

	// Task
	ChargeTask = UAbilityTask_Charge::CreateChargeTask(this, Caster, Destination);
	ChargeTask->OnAbilityTaskEnd.AddDynamic(this, &UGA_Charge::K2_EndAbility);
	ChargeTask->OnAbilityCancel.AddDynamic(this, &UGA_Charge::K2_CancelAbility);
	ChargeTask->ReadyForActivation();// 启动任务
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

