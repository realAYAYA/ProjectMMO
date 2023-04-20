// Fill out your copyright notice in the Description page of Project Settings.


#include "MAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "Net/UnrealNetwork.h"

#include "Characters/MCharacter.h"
#include "Characters/Components/MCharacterMovementComponent.h"

void UMAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
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

void UMAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHeath)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSetBase, Health, OldHeath);
}

void UMAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHeath)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSetBase, MaxHealth, OldMaxHeath);
}

void UMAttributeSetBase::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSetBase, Stamina, OldStamina);
}

void UMAttributeSetBase::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSetBase, MaxStamina, OldMaxStamina);
}

void UMAttributeSetBase::OnRep_MaxMoveSpeed(const FGameplayAttributeData& OldMaxMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSetBase, MaxMoveSpeed, OldMaxMoveSpeed);

	const AMCharacter* OwningCharacter = Cast<AMCharacter>(GetOwningActor());
	if (UCharacterMovementComponent* CharacterMovement = OwningCharacter->GetCharacterMovement())
	{
		CharacterMovement->MaxWalkSpeed = GetMaxMoveSpeed();
	}
}

void UMAttributeSetBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSetBase, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSetBase, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSetBase, MaxMoveSpeed, COND_None, REPNOTIFY_Always);
}
