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
	
	const AMCharacter* Target = Caster->CurrentTarget;
	if (!Target || !Target->GetAbilitySystemComponent())
	{
		Caster->OnAbilityFailed.Broadcast(EActivateFailCode::NoTarget);
		return EActivateFailCode::NoTarget;
	}

	FailCode = EActivateFailCode::Success;
	
	// Todo 不是敌对目标
	
	// Out of range
	const float Distance = (Caster->GetActorLocation() - Target->GetActorLocation()).Length();
	if (Distance > Range)
	{
		FailCode = EActivateFailCode::OutOfRange;
		Caster->OnAbilityFailed.Broadcast(FailCode);
		return FailCode;
	}
		
	if (Distance < MinRange)
	{
		FailCode = EActivateFailCode::ToClose;
		Caster->OnAbilityFailed.Broadcast(FailCode);
		return FailCode;
	}
	
	// 没有朝向敌人
	const FVector Dir = UKismetMathLibrary::Normal(Target->GetActorLocation() - Caster->GetActorLocation(), 0.0001);
	if (UKismetMathLibrary::Dot_VectorVector(Dir, Caster->GetActorForwardVector()) < 0.5f)
	{
		FailCode = EActivateFailCode::NoToward;
		Caster->OnAbilityFailed.Broadcast(FailCode);
		return FailCode;
	}
	
	// Todo 不在视野中

	Caster->OnAbilityFailed.Broadcast(FailCode);

	return FailCode;
}

void UGA_Charge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 根据目标位置，计算冲锋的目的地
	AMCharacter* Caster = Cast<AMCharacter>(OwnerInfo->AvatarActor);
	const AMCharacter* Target = Cast<AMCharacter>(OwnerInfo->AvatarActor)->CurrentTarget;
	UAbilitySystemComponent* TargetComponent = Target->GetAbilitySystemComponent();
	
	// 对目标施加效果
	for (const TSubclassOf<UMGameplayEffect>& Effect : EffectsToTarget)
	{
		if (!Effect.Get())
			continue;

		const FGameplayEffectContextHandle EffectContext = TargetComponent->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = TargetComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			const FActiveGameplayEffectHandle ActiveGEHandle = TargetComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			if (!ActiveGEHandle.WasSuccessfullyApplied())
				ABILITY_LOG(Log, TEXT("Ability %s faild to apply Effect to Target %s"), *GetName(), *GetNameSafe(Effect));
		}
	}

	// Task
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

