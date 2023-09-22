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

	UPROPERTY(EditDefaultsOnly, Category = ProjectM)
	FGameplayTagContainer MoveEventTag;
	
	UPROPERTY(EditDefaultsOnly, Category = ProjectM)
	FGameplayTagContainer LookEventTag;
	
	UPROPERTY(EditDefaultsOnly, Category = ProjectM)
	FGameplayTagContainer JumpEventTag;

	UPROPERTY(EditDefaultsOnly, Category = ProjectM)
	FGameplayTagContainer SprintEventTag;

	// 受到近战攻击时触发的效果
	
	// 受到近战攻击时给对方施加效果

	// 受到法术伤害时给对方施加效果

	// 近战攻击敌人时触发的效果

	// 攻击敌人时给对方施加效果

	// 施放法术时触发的效果

	const AMCharacter* GetOwnerCharacter() const { return Cast<AMCharacter>(GetOwnerActor()); }

	/**
	 * 主动键入触发的技能，这些技能往往有着如下特点，暂时只能手动实现
	 * 1.键入触发，但不知道何时停止，比如移动，需要侦听键位回调后手动取消
	 * 2.结束时机由其它模块来决定，比如跳跃，需要依赖移动组件的回调
	 */
	
	void Move();
	void MoveEnd();

	void Jump();
	void JumpEnd();

	float MovementInputX;
	
	float MovementInputY;
	
	float LookInputYaw;

	float LookInputPitch;
};
