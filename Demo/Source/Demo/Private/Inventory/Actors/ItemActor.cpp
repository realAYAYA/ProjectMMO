// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"

#include "Characters/MCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

#include "Components/PickUpComponent.h"
#include "Engine/ActorChannel.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	PickComponent = CreateDefaultSubobject<UPickUpComponent>(TEXT("SphereComponent"));
	SetRootComponent(PickComponent);
	PickComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnBeginOverlap);

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
	return;
}

void AItemActor::InitInternal()
{
	// 从配置文件中读取道具配置
	// 根据道具id从装备表中读取数据
	// 根据资产类型来设置道具模型
	
}

void AItemActor::OnPickUp(AMCharacter* InOwner)
{
	// 进包
}

void AItemActor::OnTake(AMCharacter* InOwner)
{
	ItemState = EItemState::Equipped;
	PickComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickComponent->SetGenerateOverlapEvents(false);
	ItemOwner = InOwner;
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

bool AItemActor::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	// 如果Actor里设置其他要同步的UObject，在这里进行同步操作
	//bWroteSomething = Channel->ReplicateSubobject(Channel, Bunch, RepFlags);
	return bWroteSomething;
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
	
	DOREPLIFETIME_CONDITION_NOTIFY(AItemActor, ItemState, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(AItemActor, CfgID, COND_None, REPNOTIFY_OnChanged);
}