// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Abilities/GA_Melee.h"
#include "Characters/MCharacter.h"

UGA_Melee::UGA_Melee()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

EActivateFailCode UGA_Melee::CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const
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