// Fill out your copyright notice in the Description page of Project Settings.


#include "MWeaponActor.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/MCharacter.h"
#include "Components/SphereComponent.h"
#include "Projectiles/MProjectile.h"

void AMWeaponActor::AttachWeapon(AMCharacter* TargetCharacter)
{
	return;
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	// Attach the weapon to the First Person Character
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	//AttachToComponent(Character->GetMesh1P(), AttachmentRules, AttachSocketName);
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	//Character->SetHasRifle(true);

	// Set up action bindings
	if (const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMWeaponActor::Fire);
		}
	}
}

void AMWeaponActor::Fire()
{
	return;
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (HasAuthority() && ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetItemOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			AMProjectile* NewProjectile = World->SpawnActor<AMProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (NewProjectile)
				NewProjectile->CollisionComp->IgnoreActorWhenMoving(GetItemOwner(), true);
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation(), 0.1f);
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		//UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		//if (AnimInstance != nullptr)
		{
			//AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

AMWeaponActor::AMWeaponActor()
{
}

USkeletalMeshComponent* AMWeaponActor::GetSkeletalMeshComponent() const
{
	return Cast<USkeletalMeshComponent>(GetMeshComponent());
}

void AMWeaponActor::OnPickUp(AMCharacter* InOwner)
{
	Super::OnPickUp(InOwner);

	AttachWeapon(InOwner);
}

void AMWeaponActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (Character == nullptr)
	{
		return;
	}

	if (const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}
