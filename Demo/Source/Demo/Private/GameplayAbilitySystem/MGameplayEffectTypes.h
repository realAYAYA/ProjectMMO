#pragma once
#include "GameplayEffectTypes.h"
#include "GameplayEffects/MGameplayEffect.h"
#include "MGameplayEffectTypes.generated.h"

USTRUCT()
struct DEMO_API FMGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_USTRUCT_BODY()
	
	bool IsCritical() const
	{
		return CriticalRate > 1.0f; // 获取数据
	}

	float GetCritical() const
	{
		return CriticalRate; // 获取数据
	}

	void SetCritical(const float In)
	{
		CriticalRate = In; // 添加数据
	}

	// 拷贝方法对应=号操作符
	virtual FMGameplayEffectContext* Duplicate() const override
	{
		FMGameplayEffectContext* NewContext = new FMGameplayEffectContext();
		*NewContext = *this;
		NewContext->AddActors(Actors);
		
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		
		return NewContext;
	}

	// 保证能序列化正确子类
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

protected:
	
	float CriticalRate = 1.0f;

	EMDamageType DamageType = EMDamageType::Physical;
};