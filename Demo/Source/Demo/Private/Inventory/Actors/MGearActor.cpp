// Fill out your copyright notice in the Description page of Project Settings.


#include "MGearActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/MCharacter.h"
#include "Components/PickUpComponent.h"

void AMGearActor::InitInternal()
{
	Super::InitInternal();
	
}

AMGearActor::AMGearActor()
{
	
}

void AMGearActor::OnEquipped(AActor* InOwner)
{
	ItemState = EItemState::Equipped;
	PickComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickComponent->SetGenerateOverlapEvents(false);
	
	TryGrantAbilities(GetItemOwner());
}

void AMGearActor::OnUnEquipped()
{
	ItemState = EItemState::None;
	PickComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickComponent->SetGenerateOverlapEvents(false);

	TryRemoveAbilities(GetItemOwner());
}

void AMGearActor::OnPickUp(AMCharacter* InOwner)
{
	//Super::OnPickUp(InOwner);
	OnEquipped(InOwner);
}

void AMGearActor::OnDropped()
{
	Super::OnDropped();
}

void AMGearActor::TryGrantAbilities(AActor* InOwner)
{
	if (!InOwner || !InOwner->HasAuthority())
		return;

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
	{
		TArray<UGameplayAbility> Abilities;

		for (auto& GearAbility : Abilities)
		{
			GrantedAbilitySpecHandles.Add(AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(&GearAbility)));
		}
	}
}

void AMGearActor::TryRemoveAbilities(AActor* InOwner)
{
	if (!InOwner || !InOwner->HasAuthority())
		return;

	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner))
	{
		TArray<UGameplayAbility> Abilities;

		for (auto Handle : GrantedAbilitySpecHandles)
		{
			AbilitySystemComponent->ClearAbility(Handle);
		}

		GrantedAbilitySpecHandles.Empty();
	}
}