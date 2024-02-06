#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

// 适合添加预设的Gas相关Tags
struct DEMO_API FGasGlobalTags final : public FGameplayTagNativeAdder
{
	FGameplayTag Paralysis;
	FGameplayTag Stun;
	FGameplayTag Sleep;
	FGameplayTag Fear;

	FGameplayTag Root;
	
	FGameplayTag Silence;
	
	FGameplayTag Test;
	
	virtual void AddTags() override
	{
		Paralysis	= UGameplayTagsManager::Get().AddNativeGameplayTag(TEXT("GAS.GE.Limit.Paralysis"), TEXT("ProjectMMO"));
		Stun		= UGameplayTagsManager::Get().AddNativeGameplayTag(TEXT("GAS.GE.Limit.Paralysis.Stun"), TEXT("ProjectMMO"));
		Sleep		= UGameplayTagsManager::Get().AddNativeGameplayTag(TEXT("GAS.GE.Limit.Paralysis.Sleep"), TEXT("ProjectMMO"));
		Fear		= UGameplayTagsManager::Get().AddNativeGameplayTag(TEXT("GAS.GE.Limit.Paralysis.Fear"), TEXT("ProjectMMO"));

		Root		= UGameplayTagsManager::Get().AddNativeGameplayTag(TEXT("GAS.GE.Limit.Root"), TEXT("ProjectMMO"));

		Silence		= UGameplayTagsManager::Get().AddNativeGameplayTag(TEXT("GAS.GE.Limit.Silence"), TEXT("ProjectMMO"));

		Test		= UGameplayTagsManager::Get().AddNativeGameplayTag(TEXT("GAS.Test"), TEXT("ProjectMMO"));
	}

	FORCEINLINE static const FGasGlobalTags& Get() { return GasTags; }

private:

	static FGasGlobalTags GasTags;
};
