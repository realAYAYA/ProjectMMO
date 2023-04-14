// Fill out your copyright notice in the Description page of Project Settings.


#include "MAttributeSetBase.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "Net/UnrealNetwork.h"

void UMAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
}

void UMAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHeath)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSetBase, Health, OldHeath);
}

void UMAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHeath)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSetBase, MaxHealth, OldMaxHeath);
}

void UMAttributeSetBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
}