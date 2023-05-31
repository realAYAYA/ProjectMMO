// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/Abilities/Tasks/MAbilityTask.h"
#include "AbilityTask_CastSpell.generated.h"

/**
 * 
 */
UCLASS()
class UAbilityTask_CastSpell : public UMAbilityTask
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "ProjectSS", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAbilityTask_CastSpell* CreateCastSpellTask(UGameplayAbility* OwningAbility, AMCharacter* InCharacterOwner, const float InCastTime);

	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

	virtual void TickTask(float DeltaTime) override;

protected:

	UPROPERTY()
	AMCharacter* Caster;

	int64 BeginTime;

	float CastTime;

	bool bCanStopped;
	bool bCanSilenced;
	bool bMovable;
	
};
