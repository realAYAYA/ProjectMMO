// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Characters/MCharacter.h"
#include "MAbilitySystemComponent.generated.h"

class AMCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGEAppliedDelegate, const FGameplayTag&, Tag, const float, TimeRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGERemovedDelegate, const FGameplayTag&, Tag);

/**
 * 
 */
UCLASS()
class DEMO_API UMAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	virtual void InitializeComponent() override;

	/** Whenever a duration based GE is added */
	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnGEAppliedDelegate OnGEAppliedCallback;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnGERemovedDelegate OnGERemovedCallback;
	
	// 受到近战攻击时触发的效果
	
	// 受到近战攻击时给对方施加效果

	// 受到法术伤害时给对方施加效果

	// 近战攻击敌人时触发的效果

	// 攻击敌人时给对方施加效果

	// 施放法术时触发的效果

	const AMCharacter* GetOwnerCharacter() const { return Cast<AMCharacter>(GetOwnerActor()); }
	
	void Move();
	void MoveEnd();

	void Jump();
	void JumpEnd();

protected:

	//void OnTakingMeleeAttack();

	//void OnTakingSpellAttack();

	//void OnTakingDamage();

	//void OnTakingHostileBehavior();

	void OnGEApplied(UAbilitySystemComponent* Asc, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle) const;

	void OnGERemoved(const FActiveGameplayEffect& Effect) const;
};
