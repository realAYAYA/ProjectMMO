// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MGameplayAbility.generated.h"

class UMGameplayEffect;

/**
 * 
 */
UCLASS()
class UMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Cast Condition")
	TArray<TSubclassOf<UMGameplayEffect>> EffectToMySelf;

	UPROPERTY(EditDefaultsOnly, Category = "Cast Condition")
	TArray<TSubclassOf<UMGameplayEffect>> EffectToEnemy;

	UPROPERTY(EditDefaultsOnly, Category = "Cast Condition")
	bool Friendly;

	UPROPERTY(EditDefaultsOnly, Category = "Cast Condition")
	TArray<int32> NeedItems;
	
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToRemoveOnEnd;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToJustApplyOnStart;

	TArray<FActiveGameplayEffectHandle> RemoveOnEndEffectHandles;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class AMCharacter* GetMCharacterFromActorInfo() const;
};
