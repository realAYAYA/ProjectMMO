// Fill out your copyright notice in the Description page of Project Settings.


#include "GEEC_LifeSteal.h"

#include "Characters/MCharacter.h"
#include "GameplayAbilitySystem/AttributeSets/MAttributeSet.h"
#include "GameplayAbilitySystem/GameplayEffects/MGameplayEffect.h"

struct FEffectStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FEffectStatics()
	{
		// Capture the Target's DefensePower attribute. Do not snapshot it,
		// because we want to use the health value at the moment we apply the execution.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, Health, Target, false);

		// Capture the Source's AttackPower. We do want to snapshot this at the moment we create the GameplayEffectSpec that will execute the damage.
		// (imagine we fire a projectile: we create the GE Spec when the projectile is fired. When it hits the target, we want to use the AttackPower at the moment
		// the projectile was launched, not when it hits).
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, Health, Source, true);

		// Also capture the source's raw Damage, which is normally passed in directly via the execution
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMAttributeSet, Health, Source, true);
	}
};

static const FEffectStatics& EffectStatics()
{
	static FEffectStatics Statics;
	return Statics;
}

UMGameplayEffectExecutionCalculation::UMGameplayEffectExecutionCalculation()
{
	RelevantAttributesToCapture.Add(EffectStatics().HealthDef);
}

AMCharacter* UMGameplayEffectExecutionCalculation::GetTarget(const FGameplayEffectCustomExecutionParameters& ExecutionParams)
{
	const UAbilitySystemComponent* TargetAbilitySystem = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetAbilitySystem ? TargetAbilitySystem->GetAvatarActor() : nullptr;
	return Cast<AMCharacter>(TargetActor);
}

AMCharacter* UMGameplayEffectExecutionCalculation::GetSource(const FGameplayEffectCustomExecutionParameters& ExecutionParams)
{
	const UAbilitySystemComponent* SourceAbilitySystem = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceAbilitySystem ? SourceAbilitySystem->GetAvatarActor() : nullptr;
	return Cast<AMCharacter>(SourceActor);
}

void UMGameplayEffectExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	Cast<UMGameplayEffect>(Spec.Def);
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	AMCharacter* Target = GetTarget(ExecutionParams);
	AMCharacter* Source = GetTarget(ExecutionParams);
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(EffectStatics().HealthDef, EvaluationParameters, Armor);
	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(EffectStatics().HealthDef, EvaluationParameters, Damage);
	
	if (const float MitigatedDamage = Damage - Armor > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(EffectStatics().HealthProperty, EGameplayModOp::Additive, -MitigatedDamage));
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(EffectStatics().HealthProperty, EGameplayModOp::Additive, -10.0f));
	}
	
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
}
