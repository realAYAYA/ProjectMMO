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

class UMAbilitySystemComponent;
class UMAttributeSet;

class UGameplayAbility;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMoveInput, float, X, float, Y);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLookInput, float, X, float, Y);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJumpInput, float, V);

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
	 * PlayerState
	*/
	
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = ProjectSS)
	AMCharacter* GetCurrentTarget() const;

	UFUNCTION(BlueprintCallable, Server, Reliable, Category = ProjectSS)
	void SetCurrentTarget(AMCharacter* NewTarget);

protected:
	
	/**  当前锁定目标*/
	UPROPERTY(Replicated)
	AMCharacter* CurrentTarget;

	// Todo 测试数据，最后要删
	UPROPERTY(BlueprintReadOnly, Replicated, Category = ProjectSS)
	FString MyName;

	UPROPERTY(Replicated)
	FCharacterData CharacterData;

	UPROPERTY(EditDefaultsOnly)
	class UMCharacterDataAsset* CharacterDataAsset;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = ProjectSS)
	void SetMyName(const FString& InName);

	
	/**
	 * GameAbilitySystem
	 */
public:

	UFUNCTION(BlueprintCallable, Category = ProjectSS)
	const UMAttributeSet* GetAttributeSet() const;
	
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
	UMAttributeSet* AttributeSet;

	// 移动限制Tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ProjectSS)
	FGameplayTagContainer MoveLimitTags;

	FDelegateHandle MaxMovementSpeedChangedDelegatedHandle;
	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);

	
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
