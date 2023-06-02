// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystem/Components/MAbilitySystemComponent.h"

void UMAbilitySystemComponent::Move()
{
	TryActivateAbilitiesByTag(MoveEventTag, true);
}

void UMAbilitySystemComponent::MoveEnd()
{
	MovementInputX = 0;
	MovementInputY = 0;
	
	CancelAbilities(&MoveEventTag);
}

void UMAbilitySystemComponent::Jump()
{
	TryActivateAbilitiesByTag(JumpEventTag, true);
}

void UMAbilitySystemComponent::JumpEnd()
{
	CancelAbilities(&JumpEventTag);
}
