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
	 * PlayerState
	*/

public:
	UFUNCTION(BlueprintCallable, Category = ProjectSS)
	AMCharacter* GetCurrentTarget() const;

	UFUNCTION(Client, Reliable, Category = ProjectSS)
	void SetCurrentTarget(AMCharacter* NewTarget);

protected:
	/**  */
	UPROPERTY(Replicated)
	AMCharacter* CurrentTarget;

	UPROPERTY(Replicated)
	bool bHasWeapon;

	// Todo 测试数据，最后要删
	UPROPERTY(BlueprintReadOnly, Category = PlayerData, Replicated)
	FString MyName = "None";

public:
	UFUNCTION(BlueprintCallable, Category = ATest)
	void SetMyName(const FString& InName) { MyName = InName; }

	
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

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	FGameplayTag JumpEventTag;

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	FGameplayTagContainer InAirTags;

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	FGameplayTagContainer SprintTags;

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

protected:
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for jump input */
	void TryJump(const FInputActionValue& Value);

	virtual void Landed(const FHitResult& Hit) override;

	/** Called for sprint input */
	void BeginSprint(const FInputActionValue& Value);
	void EndSprint(const FInputActionValue& Value);


	/** Skill */
	void TryActiveAbility(const FInputActionValue& Value);
	void TryActiveAbility1(const FInputActionValue& Value);
	void TryActiveAbility2(const FInputActionValue& Value);
	void TryActiveAbility3(const FInputActionValue& Value);
	void TryActiveAbility4(const FInputActionValue& Value);

	UPROPERTY()
	TMap<int32, FGameplayTag> InputSkillMap;
};
