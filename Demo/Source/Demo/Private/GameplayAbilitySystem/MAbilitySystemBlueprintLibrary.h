#pragma once
#include "MGameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "MAbilitySystemBlueprintLibrary.generated.h"

/** Blueprint library for ability system. Many of these functions are useful to call from native as well */
UCLASS()
class DEMO_API UMAbilitySystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// -------------------------------------------------------------------------------
	//		GameplayEffectContext
	// -------------------------------------------------------------------------------
	
	/** Returns critical rate inside the effect context */
	UFUNCTION(BlueprintPure, Category = "ProjectM", Meta = (DisplayName = "GetCritical"))
	static float EffectContextGetCritical(FGameplayEffectContextHandle EffectContext);
	
	/** Returns true if there is a critical hit inside the effect context */
	UFUNCTION(BlueprintPure, Category = "ProjectM", Meta = (DisplayName = "IsCritical"))
	static bool EffectContextIsCritical(const FGameplayEffectContextHandle& EffectContext);

	/** Returns true if there is a critical hit inside the effect context */
	UFUNCTION(BlueprintPure, Category = "ProjectM", Meta = (DisplayName = "GetDamageType"))
	static EMDamageType EffectContextGetDamageType(FGameplayEffectContextHandle& EffectContext);

	// -------------------------------------------------------------------------------
	//		GameplayCue
	// -------------------------------------------------------------------------------

	/** Returns critical rate inside the effect context */
	UFUNCTION(BlueprintPure, Category = "ProjectM")
	static float GetCritical(const FGameplayCueParameters& EffectContext);

	/** Returns true if there is a critical hit inside the effect context */
	UFUNCTION(BlueprintPure, Category = "ProjectM")
	static bool IsCritical(const FGameplayCueParameters& EffectContext);
};