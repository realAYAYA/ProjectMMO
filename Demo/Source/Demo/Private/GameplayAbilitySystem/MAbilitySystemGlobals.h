#pragma once
#include "AbilitySystemGlobals.h"
#include "MGameplayEffectTypes.h"
#include "MAbilitySystemGlobals.generated.h"

/** Holds global data for the ability system. Can be configured per project via config file */
UCLASS(config=Game)
class DEMO_API UMAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override
	{
		return new FMGameplayEffectContext();
	}
	
};