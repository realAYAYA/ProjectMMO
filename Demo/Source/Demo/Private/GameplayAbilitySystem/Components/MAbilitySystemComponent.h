// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Characters/MCharacter.h"
#include "MAbilitySystemComponent.generated.h"

class AMCharacter;
/**
 * 
 */
UCLASS()
class DEMO_API UMAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	FGameplayTag MoveEventTag;

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	FGameplayTag JumpEventTag;

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	FGameplayTagContainer MoveLimitTags;

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	FGameplayTagContainer SilenceTags;

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	FGameplayTagContainer StunnedTags;

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	FGameplayTagContainer CastingTags;

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	FGameplayTagContainer BusyingTag;

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	FGameplayTagContainer InAirTags;

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	FGameplayTagContainer SprintTags;

	UFUNCTION(BlueprintCallable, Category = ProjectSS)
	bool CanMove() const;

	UFUNCTION(BlueprintCallable, Category = ProjectSS)
	bool CanCastSpell() const;

	UFUNCTION(BlueprintCallable, Category = ProjectSS)
	bool CanUseAbility() const;

	const AMCharacter* GetOwnerCharacter() const { return Cast<AMCharacter>(GetOwnerActor()); }
};
