// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/GA_CastSpell.h"
#include "Characters/MCharacter.h"
#include "Tasks/AbilityTask_CastSpell.h"

#include "AbilitySystemComponent.h"

UGA_CastSpell::UGA_CastSpell()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	Range = 1500;

	SpellTask = nullptr;
}

EActivateFailCode UGA_CastSpell::CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const
{
	const AMCharacter* Caster = Cast<AMCharacter>(ActorInfo.AvatarActor.Get());
	
	EActivateFailCode FailCode = Super::CanActivateCondition(ActorInfo);
	if (FailCode != EActivateFailCode::Success)
	{
		Caster->OnAbilityFailed.Broadcast(FailCode);
		return FailCode;
	}
	
	Caster->OnAbilityFailed.Broadcast(FailCode);
	
	return FailCode;
}

void UGA_CastSpell::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
	
	AMCharacter* Caster = GetMCharacterFromActorInfo();
	SpellTask = UAbilityTask_CastSpell::CreateCastSpellTask(this, Caster, CastTime);
	SpellTask->OnAbilityTaskEnd.AddDynamic(this, &UGA_CastSpell::K2_EndAbility);
	SpellTask->OnAbilityCancel.AddDynamic(this, &UGA_CastSpell::K2_CancelAbility);
	SpellTask->ReadyForActivation();// 启动任务
}

void UGA_CastSpell::CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	if (SpellTask)
		SpellTask->EndTask();
}

void UGA_CastSpell::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	if (CanActivateCondition(*ActorInfo) != EActivateFailCode::Success)
	{
		bWasCancelled = true;
	}

	if (SpellTask)
		SpellTask->EndTask();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
