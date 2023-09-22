// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGearActor.h"
#include "MWeaponActor.generated.h"

class AMCharacter;

/**
 * 
 */
UCLASS()
class AMWeaponActor : public AMGearActor
{
	GENERATED_BODY()


	/** 测试数据开头*/
public:

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = ATest)
	void AttachWeapon(AMCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = ATest)
	void Fire();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = ATest)
	TSubclassOf<class AMProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ATest)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ATest)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ATest)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ATest, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ATest, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** 测试数据结尾*/
	
public:
	AMWeaponActor();

	UFUNCTION(BlueprintPure, Category = ProjectM)
	USkeletalMeshComponent* GetSkeletalMeshComponent() const;

	virtual void OnPickUp(AMCharacter* InOwner) override;

protected:

	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** The Character holding this weapon*/
	UPROPERTY()
	AMCharacter* Character;
};
