// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "MGameTypes.h"
#include "Abilities/GameplayAbility.h"
#include "MCharacter.generated.h"

class USpringArmComponent;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

class UMAbilitySystemComponentBase;

class UGameplayAbility;
class UGameplayEffect;

UCLASS()
class DEMO_API AMCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Third person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCameraComponent;

	/** Third person camera arm */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

public:
	
	/** Returns FirstPersonCameraComponent sub-object **/
	UCameraComponent* GetThirdPersonCameraComponent() const { return ThirdPersonCameraComponent; }

	/**
	 * Input
	 */
public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(BlueprintReadOnly, Category = Weapon, Replicated)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(const bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle() const;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for jump input */
	void TryJump(const FInputActionValue& Value);

	virtual void Landed(const FHitResult& Hit) override;

	/** Called for crouch input */
	void BeginCrouch(const FInputActionValue& Value);
	void EndCrouch(const FInputActionValue& Value);

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	/** Called for sprint input */
	void BeginSprint(const FInputActionValue& Value);
	void EndSprint(const FInputActionValue& Value);

	/**
	 * GameAbilitySystem
	 */
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	bool ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext);

protected:

	void GiveAbilities();
	void ApplyStartupEffects();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(Transient)
	class UMAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag JumpEventTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer InAirTags;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer CrouchTags;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer SprintTags;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> CrouchStateEffect;

	FDelegateHandle MaxMovementSpeedChangedDelegatedHandle;
	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);

public:
	UFUNCTION(BlueprintCallable)
	FCharacterData GetCharacterData() const { return CharacterData; }

	UFUNCTION(BlueprintCallable)
	void SetCharacterData(const FCharacterData& InCharacterData);

protected:

	UPROPERTY(ReplicatedUsing = OnRep_CharacterData)
	FCharacterData CharacterData;

	UFUNCTION()
	void OnRep_CharacterData();

	virtual void InitFromCharacterData(const FCharacterData& InCharacterData, const bool bFromReplication = false);

	UPROPERTY(EditDefaultsOnly)
	class UMCharacterDataAsset* CharacterDataAsset;

	/**
	 * Default
	 */
public:
	// Sets default values for this character's properties

	//AMCharacter();
	AMCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
