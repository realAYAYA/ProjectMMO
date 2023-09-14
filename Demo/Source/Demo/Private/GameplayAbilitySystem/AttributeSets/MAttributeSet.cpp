// Fill out your copyright notice in the Description page of Project Settings.


#include "MAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "Net/UnrealNetwork.h"

#include "Characters/MCharacter.h"
#include "Characters/Components/MCharacterMovementComponent.h"

void UMAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxMoveSpeedAttribute())
	{
		const AMCharacter* OwningCharacter = Cast<AMCharacter>(GetOwningActor());
		if (UCharacterMovementComponent* CharacterMovement = OwningCharacter->GetCharacterMovement())
		{
			CharacterMovement->MaxWalkSpeed = GetMaxMoveSpeed();
		}
	}
}

void UMAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Mana, OldMana);
}

void UMAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxMana, OldMaxMana);
}

void UMAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Energy, OldEnergy);
}

void UMAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxEnergy, OldMaxEnergy);
}

void UMAttributeSet::OnRep_Rage(const FGameplayAttributeData& OldRage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Rage, OldRage);
}

void UMAttributeSet::OnRep_MaxRage(const FGameplayAttributeData& OldMaxRage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxRage, OldMaxRage);
}

void UMAttributeSet::OnRep_CastSpeed(const FGameplayAttributeData& OldCastSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, CastSpeed, OldCastSpeed);
}

void UMAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHeath)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Health, OldHeath);
}

void UMAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHeath)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxHealth, OldMaxHeath);
}

void UMAttributeSet::OnRep_MaxMoveSpeed(const FGameplayAttributeData& OldMaxMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxMoveSpeed, OldMaxMoveSpeed);
}

void UMAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, Health, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MaxHealth, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MaxMoveSpeed, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, Mana, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MaxMana, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, Energy, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MaxEnergy, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, Rage, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MaxRage, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, CastSpeed, COND_None, REPNOTIFY_OnChanged);
}
