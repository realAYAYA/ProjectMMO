#include "MAbilitySystemBlueprintLibrary.h"

float UMAbilitySystemBlueprintLibrary::GetCritical(const FGameplayCueParameters& EffectContext)
{
	return EffectContextGetCritical(EffectContext.EffectContext);
}

bool UMAbilitySystemBlueprintLibrary::IsCritical(const FGameplayCueParameters& EffectContext)
{
	return EffectContextIsCritical(EffectContext.EffectContext);
}

float UMAbilitySystemBlueprintLibrary::EffectContextGetCritical(FGameplayEffectContextHandle EffectContext)
{
	if (const FMGameplayEffectContext* MEffectContext = static_cast<FMGameplayEffectContext*>(EffectContext.Get()))
		return MEffectContext->GetCritical();

	return 1.0f;
}

bool UMAbilitySystemBlueprintLibrary::EffectContextIsCritical(const FGameplayEffectContextHandle& EffectContext)
{
	return EffectContextGetCritical(EffectContext) > 1.0f;
}

EMDamageType UMAbilitySystemBlueprintLibrary::EffectContextGetDamageType(FGameplayEffectContextHandle EffectContext)
{
	if (const FMGameplayEffectContext* MEffectContext = static_cast<FMGameplayEffectContext*>(EffectContext.Get()))
		return MEffectContext->GetDamageType();
	
	return EMDamageType::Physical;
}

