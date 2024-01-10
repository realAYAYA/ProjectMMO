#pragma once
#include "GameplayEffectTypes.h"
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

	// 该方法必须被重写 应当在方法中创建子类并返回子类指针
	// 原因在若不重写的话，复制之后就变回 父类了
	// 此时如果强制转换，用子类指针父类 自然会报空指针
	// 这类错误的强制转换 能够逃过 if(nullptr) 判定！！！
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
		
		NewContext->CriticalRate = CriticalRate; // not necessary
		
		return NewContext;
	}

	// 注释上说子类必须重写该方法，保证能序列化
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

protected:
	
	float CriticalRate = 1.0f;
};