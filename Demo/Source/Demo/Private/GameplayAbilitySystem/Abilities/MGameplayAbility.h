// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MGameplayAbility.generated.h"

class UMGameplayEffect;

UENUM(BlueprintType)
enum class ETargetType : uint8
{
	Self		UMETA(Displayname = "Self"),
	Hostile		UMETA(Displayname = "Hostile"),
	Friendly	UMETA(Displayname = "Friendly"),
};

/**
 * 
 */
UCLASS()
class UMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	
protected:

	/** 技能等级*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectSS")
	int32 Level = 1;

	/** 射程*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectSS")
	float Range = 20.0f;

	/** 最小射程*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectSS")
	float MinRange = 0.0f;

	/** 对目标施加效果*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectSS")
	TSubclassOf<UMGameplayEffect> EffectToTarget;

	/** 技能目标类型*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectSS")
	ETargetType TargetType;

	/** 消耗品*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectSS")
	TArray<int32> NeedItems;

	/** 技能激活时赋予效果，结束时主动移除，配合CancelAbility()，适合技能及其效果同时存在的情况*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectSS")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToRemoveOnEnd;

	/** 技能激活时赋予效果，但结束时并不会主动移除*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectSS")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToJustApplyOnStart;

private:
	TArray<FActiveGameplayEffectHandle> RemoveOnEndEffectHandles;

	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	class AMCharacter* GetMCharacterFromActorInfo() const;
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanActivateCondition(const FGameplayAbilityActorInfo* ActorInfo) const;
};
