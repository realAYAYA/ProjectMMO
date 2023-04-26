// Fill out your copyright notice in the Description page of Project Settings.


#include "MGearActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/MCharacter.h"
#include "Components/PickUpComponent.h"

void AMGearActor::InitInternal()
{
	Super::InitInternal();

	// 根据道具id从装备表中读取数据
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

void AMGearActor::OnEquipped(AActor* InOwner)
{
	ItemState = EItemState::Equipped;
	PickComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickComponent->SetGenerateOverlapEvents(false);
	
	//AttachToActor(InOwner, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Socket"));
	
	TryGrantAbilities(GetItemOwner());
}

void AMGearActor::OnUnEquipped()
{
	ItemState = EItemState::None;
	PickComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickComponent->SetGenerateOverlapEvents(false);

	TryRemoveAbilities(GetItemOwner());
}

void AMGearActor::OnPickUp(AActor* InOwner)
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