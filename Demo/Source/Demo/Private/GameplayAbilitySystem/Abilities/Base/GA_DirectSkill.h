// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_Base.h"
#include "GA_DirectSkill.generated.h"

/**
 * 瞬发技能，包括读条后单次施法技能（火球术，治疗术），
 * 它不适合用于带有“持续释放时间”的这种引导性技能（比如暗影鞭笞，吸取生命，暴风雪）
 */
UCLASS()
class UGA_DirectSkill : public UGA_Base
{
	GENERATED_BODY()

public:

	/** 技能目标类型*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	ETargetType TargetType = ETargetType::HostileOnly;

	/** 技能起手时对目标施加效果*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	TArray<TSubclassOf<UMGameplayEffect>> EffectsToTargetOnStart;

	/** 技能结束时对目标施加效果*/
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	TArray<TSubclassOf<UMGameplayEffect>> EffectsToTargetOnEnd;

protected:

	UPROPERTY(Transient)
	TObjectPtr<AMCharacter> Target = nullptr;

public:
	
	virtual EActivateFailCode CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const override;
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

};
