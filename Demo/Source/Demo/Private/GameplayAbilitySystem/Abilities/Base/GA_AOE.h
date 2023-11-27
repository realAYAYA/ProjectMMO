// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_Base.h"
#include "GA_AOE.generated.h"

/**
 * 瞬发型AOE
 */
UCLASS()
class UGA_AOE : public UGA_Base
{
	GENERATED_BODY()

public:
	
	/** 作用半径 */
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	int32 Radius = 360;
	
	/** 扇形张角, 360为一个圆 */
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	int32 FanAngle = 360;

	/** 技能起手时对友方目标施加效果 */
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	TArray<TSubclassOf<UMGameplayEffect>> EffectsToFriendOnStart;

	/** 技能起手时对友方目标施加效果 */
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	TArray<TSubclassOf<UMGameplayEffect>> EffectsToFriendOnEnd;

	/** 技能起手时对敌对目标施加效果 */
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	TArray<TSubclassOf<UMGameplayEffect>> EffectsToHostileOnStart;

	/** 技能起手时对敌对目标施加效果 */
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	TArray<TSubclassOf<UMGameplayEffect>> EffectsToHostileOnEnd;

protected:
	
	virtual EActivateFailCode CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const override;
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
private:

	UPROPERTY(Transient)
	FVector TargetCenter = FVector(0, 0, 0);
};
