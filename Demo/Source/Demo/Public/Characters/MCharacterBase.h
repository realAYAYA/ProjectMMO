// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Abilities/GameplayAbility.h"
#include "MCharacterBase.generated.h"

class UMAbilitySystemComponentBase;
class UMAttributeSetBase;

class UGameplayAbility;
class UGameplayEffect;

UCLASS()
class DEMO_API AMCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMCharacterBase();

	/**
	 * GameAbilitySystem
	 */
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	bool ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle InEffectContext);

protected:
	void InitializeAttributes();
	void GiveAbilities();
	void ApplyStartupEffects();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributeSet;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	UPROPERTY(EditDefaultsOnly)
	UMAbilitySystemComponentBase* AbilitySystemComponent;

	UPROPERTY(Transient)
	UMAttributeSetBase* AttributeSet;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
