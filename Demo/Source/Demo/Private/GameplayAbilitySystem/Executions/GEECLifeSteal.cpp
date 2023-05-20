// Fill out your copyright notice in the Description page of Project Settings.


#include "GEECLifeSteal.h"

#include "GameplayAbilitySystem/AttributeSets/MAttributeSet.h"

struct FEffectStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FEffectStatics()
	{
		// Capture the Target's DefensePower attribute. Do not snapshot it,
		// because we want to use the health value at the moment we apply the execution.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, Health, Target, true);

		// Capture the Source's AttackPower. We do want to snapshot this at the moment we create the GameplayEffectSpec that will execute the damage.
		// (imagine we fire a projectile: we create the GE Spec when the projectile is fired. When it hits the target, we want to use the AttackPower at the moment
		// the projectile was launched, not when it hits).

		// Also capture the source's raw Damage, which is normally passed in directly via the execution
	}
};

static const FEffectStatics& EffectStatics()
{
	static FEffectStatics Statics;
	return Statics;
}

UGEECLifeSteal::UGEECLifeSteal()
{
	RelevantAttributesToCapture.Add(EffectStatics().HealthDef);
}

void UGEECLifeSteal::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystem = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystem = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* TargetActor = TargetAbilitySystem ? TargetAbilitySystem->GetAvatarActor() : nullptr;
	AActor* SourceActor = SourceAbilitySystem ? SourceAbilitySystem->GetAvatarActor() : nullptr;
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(EffectStatics().HealthDef, EvaluationParameters, Armor);
	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(EffectStatics().HealthDef, EvaluationParameters, Damage);
	
	float UnmitigatedDamage = Damage;
	float MitigatedDamage = UnmitigatedDamage - Armor;
	if (MitigatedDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(EffectStatics().HealthProperty, EGameplayModOp::Additive, -MitigatedDamage));
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(EffectStatics().HealthProperty, EGameplayModOp::Additive, -10.0f));
	}
	
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
}
