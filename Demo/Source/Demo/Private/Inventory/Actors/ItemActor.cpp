// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Characters/MCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

#include "Components/PickUpComponent.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);
	SetReplicateMovement(true);

	PickComponent = CreateDefaultSubobject<UPickUpComponent>(TEXT("SphereComponent"));
	PickComponent->SetupAttachment(RootComponent);
	PickComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnBeginOverlap);
}

void AItemActor::Init()
{
	InitInternal();
}

void AItemActor::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		AMCharacter* Character = Cast<AMCharacter>(OtherActor);
		if(Character != nullptr)
		{
			// Notify that the actor is being picked up
			PickComponent->OnPickUp.Broadcast(Character);

			// Unregister from the Overlap Event so it is no longer triggered
			PickComponent->OnComponentBeginOverlap.RemoveAll(this);
		}

		OnPickUp(OtherActor);
		//FGameplayEventData EventPayload;
		//EventPayload.Instigator = this;
		//EventPayload.OptionalObject;// = ;
		//EventPayload.EventTag;// = UInventory;

		//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, EventPayload.EventTag, EventPayload);
	}
}

void AItemActor::InitInternal()
{
	// 从配置文件中读取道具配置
}

void AItemActor::OnPickUp(AActor* InOwner)
{
	// 进包
}

void AItemActor::OnTake(AActor* InOwner)
{
	ItemState = EItemState::Equipped;
	PickComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickComponent->SetGenerateOverlapEvents(false);
}

void AItemActor::OnDropped()
{
	ItemState = EItemState::Dropped;

	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	if (AActor* ActorOwner = GetOwner())
	{
		const FVector Location = GetActorLocation();
		const FVector Forward = ActorOwner->GetActorForwardVector();
		const float DropDist = 100.f;
		const float DropTraceDist = 10000.0f;
		const FVector TraceStart = Location + Forward * DropDist;
		const FVector TraceEnd = TraceStart - FVector::UpVector * DropTraceDist;

		TArray<AActor*> ActorsToIgnore = { ActorOwner };

		FHitResult TraceHit;
		static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ShowDebugInventory"));
		const bool bShowInventory = CVar->GetInt() > 0;
		
		FVector TargetLocation = TraceEnd;

		const EDrawDebugTrace::Type DebugDrawType = bShowInventory ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		if (UKismetSystemLibrary::LineTraceSingleByProfile(
			this,
			TraceStart, TraceEnd,
			TEXT("WorldStatic"),
			true,
			ActorsToIgnore,
			DebugDrawType,
			TraceHit,
			true))
		{
			if (TraceHit.bBlockingHit)
				TargetLocation = TraceHit.Location;
		}

		SetActorLocation(TargetLocation);
		PickComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		PickComponent->SetGenerateOverlapEvents(true);
	}
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemActor::OnRep_ItemState()
{
	switch (ItemState)
	{
	case EItemState::Equipped:
		break;
	default:
			
		break;
	}
}

void AItemActor::OnRep_ItemID()
{
	
}

void AItemActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemActor, ItemState);
}