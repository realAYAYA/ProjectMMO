// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/GearActor.h"

#include "Kismet/KismetSystemLibrary.h"

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
}

void AGearActor::OnUnEquipped()
{
	ItemState = EItemState::None;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetGenerateOverlapEvents(false);
}

void AGearActor::OnDropped()
{
	Super::OnDropped();
}