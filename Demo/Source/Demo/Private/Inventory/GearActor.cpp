// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/GearActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/MCharacter.h"
#include "Components/PickUpComponent.h"

void AGearActor::InitInternal()
{
	Super::InitInternal();

	// 根据资产类型来设置道具模型
	if (false)
	{
		USkeletalMeshComponent * SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass(), TEXT("MeshComponent"));
		if (SkeletalMeshComponent)
		{
			SkeletalMeshComponent->RegisterComponent();
			SkeletalMeshComponent->SetSkeletalMesh(nullptr);// 设置资产
			SkeletalMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			MeshComponent = SkeletalMeshComponent;
		}
	}
	else if (true)
	{
		UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), TEXT("MeshComponent"));
		if (StaticMeshComponent)
		{
			StaticMeshComponent->RegisterComponent();
			StaticMeshComponent->SetStaticMesh(nullptr);// 设置资产
			StaticMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			MeshComponent = StaticMeshComponent;
		}
	}
}

void AGearActor::OnEquipped(AActor* InOwner)
{
	ItemState = EItemState::Equipped;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetGenerateOverlapEvents(false);
	
	TryGrantAbilities(GetItemOwner());
}

void AGearActor::OnUnEquipped()
{
	ItemState = EItemState::None;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetGenerateOverlapEvents(false);

	TryRemoveAbilities(GetItemOwner());
}

void AGearActor::OnPickUp(AActor* InOwner)
{
	//Super::OnPickUp(InOwner);
	OnEquipped(InOwner);
}

void AGearActor::OnDropped()
{
	Super::OnDropped();
}

void AGearActor::TryGrantAbilities(AActor* InOwner)
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

void AGearActor::TryRemoveAbilities(AActor* InOwner)
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