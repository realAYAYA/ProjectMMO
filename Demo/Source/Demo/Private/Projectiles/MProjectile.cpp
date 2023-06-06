// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/MProjectile.h"

#include "AbilitySystemComponent.h"
#include "Characters/MCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMProjectile::AMProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicatingMovement(true);
	
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMProjectile::OnHit);// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	//ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}

void AMProjectile::Initialize(AMCharacter* InCaster, AMCharacter* InTarget)
{
	if (!HasAuthority())
		return;
	
	Caster = InCaster;
	Target = InTarget;

	if (Caster)
		CollisionComp->IgnoreActorWhenMoving(InCaster, true);
}

void AMProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
 {
	// Only add impulse and destroy projectile if we hit a physics
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		AMCharacter* Character = Cast<AMCharacter>(OtherActor);
		if (HasAuthority() && Character)
		{
			// 施加效果
			for (const TSubclassOf<UGameplayEffect>& Effect : Effects)
			{
				FGameplayEffectContextHandle EffectContextHandle = Character->GetAbilitySystemComponent()->MakeEffectContext();
				EffectContextHandle.AddSourceObject(Character);
				Character->ApplyGameplayEffectToSelf(Effect, EffectContextHandle);
			}

			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void AMProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (!HasAuthority())
		return;
	
	// 追踪逻辑
	if (Target)
	{
		TargetLocation = Target->GetActorLocation();
	}
	else
	{
		if (GetActorLocation().Equals(TargetLocation, 1.0f))
			Destroy();
	}
	
	const FVector Dir = (TargetLocation - GetActorLocation()).GetSafeNormal();
	SetActorRotation(Dir.Rotation());
	ProjectileMovement->Velocity = Dir * 800.0f;
}

