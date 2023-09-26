// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "MGameTypes.h"
#include "Abilities/GameplayAbility.h"
#include "MCharacter.generated.h"

class AMPlayerController;
class AMPlayerState;
class USpringArmComponent;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

class UMAbilitySystemComponent;
class UMAttributeSet;

class UGameplayAbility;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoleNameChanged, FString, InName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentChanged, AMCharacter*, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMoveInput, float, X, float, Y);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLookInput, float, X, float, Y);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJumpInput, float, V);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, V);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChanged, float, V);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChanged, float, V);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChanged, float, V);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRageChanged, float, V);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxRageChanged, float, V);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnergyChanged, float, V);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxEnergyChanged, float, V);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityFailed, EActivateFailCode, FailCode);

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

	/**
	 * Default
	 */
public:
	
	// Sets default values for this character's properties
	AMCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ProjectM")
	AMPlayerState* GetMPlayerState() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ProjectM")
	AMPlayerController* GetMPlayerController() const;
	
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void OnRep_PlayerState() override;

	virtual void PostInitializeComponents() override;

	UCameraComponent* GetThirdPersonCameraComponent() const { return ThirdPersonCameraComponent; }
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * Network
	*/
public:
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RoleName, Category = "ProjectM")
	FString RoleName;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnRoleNameChanged OnRoleNameChanged;

	/** 当前锁定目标*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentTarget, Category = "ProjectM")
	AMCharacter* CurrentTarget;

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ProjectM")
	void SetCurrentTarget(AMCharacter* NewTarget);

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnCurrentChanged OnCurrentChanged;

protected:

	UFUNCTION()
	void OnRep_RoleName() const;

	UFUNCTION()
	void OnRep_CurrentTarget() const;

private:
	
	/**
	 * GameAbilitySystem
	 */
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMAbilitySystemComponent* AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	bool ApplyGameplayEffectToSelf(
		const TSubclassOf<UGameplayEffect> Effect,
		const FGameplayEffectContextHandle& InEffectContext) const;

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	const UMAttributeSet* GetAttributeSet() const;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnAbilityFailed OnAbilityFailed;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnMaxHealthChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnManaChanged OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnMaxManaChanged OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnRageChanged OnRageChanged;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnMaxRageChanged OnMaxRageChanged;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnEnergyChanged OnEnergyChanged;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnMaxEnergyChanged OnMaxEnergyChanged;

protected:

	UPROPERTY(Replicated)
	FCharacterData CharacterData;

	UPROPERTY(EditDefaultsOnly)
	class UMCharacterDataAsset* CharacterDataAsset;

	void GiveAbilities();
	
	void ApplyStartupEffects();

	// 移动限制Tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	FGameplayTagContainer MoveLimitTags;

	FDelegateHandle MaxMovementSpeedChangedDelegatedHandle;
	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);

private:

	UPROPERTY(Transient)
	UMAttributeSet* AttributeSet;
	
	/**
	 * Input & Control
	 */
public:
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputAction1;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputAction2;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputAction3;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputAction4;

	UPROPERTY(BlueprintAssignable)
	FOnMoveInput OnMoveInput;

	UPROPERTY(BlueprintAssignable)
	FOnLookInput OnLookInput;

	UPROPERTY(BlueprintAssignable)
	FOnJumpInput OnJumpInput;

protected:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void MoveEnd(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for jump input */
	void TryJump(const FInputActionValue& Value);

	virtual void Landed(const FHitResult& Hit) override;
	
	/** Skill */
	void TryActiveAbility(const FInputActionValue& Value);

	UPROPERTY()
	TMap<int32, FGameplayTag> InputSkillMap;

};
