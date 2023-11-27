// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/MCharacter.h"
#include "GameplayAbilitySystem/Abilities/Base/MAbilityTask.h"
#include "AbilityTask_Charge.generated.h"

class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class UAbilityTask_Charge : public UMAbilityTask
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "ProjectM", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAbilityTask_Charge* CreateChargeTask(UGameplayAbility* OwningAbility, AMCharacter* InCharacterOwner, FVector InDestination);

	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

	virtual void TickTask(float DeltaTime) override;

protected:

	UPROPERTY()
	AMCharacter* Caster;

	FVector Destination;

	float CastTime;
};
