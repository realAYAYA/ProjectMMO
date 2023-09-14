// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "MAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class UMAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Strength)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Intelligence)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Agility)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData Spirit;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Spirit)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes", ReplicatedUsing = OnRep_MaxMoveSpeed)
	FGameplayAttributeData MaxMoveSpeed;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxMoveSpeed)
	
	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData HealthRecovery;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, HealthRecovery)

	// 治疗量，比如致死打击可以造成治疗量的减少
	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData HealRatio;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, HealRatio)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData ManaRecovery;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, ManaRecovery)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes", ReplicatedUsing = OnRep_Energy)
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Energy)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes", ReplicatedUsing = OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxEnergy)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData EnergyRecovery;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, EnergyRecovery)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes", ReplicatedUsing = OnRep_Rage)
	FGameplayAttributeData Rage;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Rage)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes", ReplicatedUsing = OnRep_MaxRage)
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxRage)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData MeleeAttackIntensity;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MeleeAttackIntensity)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData RangedAttackIntensity;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, RangedAttackIntensity)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, AttackSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData MagicIntensity;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MagicIntensity)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData HealIntensity;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, HealIntensity)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes", ReplicatedUsing = OnRep_CastSpeed)
	FGameplayAttributeData CastSpeed;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, CastSpeed)
	
	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData PhyCriticalRate;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, PhyCriticalRate)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData MagicCriticalRate;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MagicCriticalRate)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData HitRate;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, HitRate)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData ArcaneDamage;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, ArcaneDamage)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData FrostDamage;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, FrostDamage)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData FlameDamage;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, FlameDamage)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData NaturalDamage;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, NaturalDamage)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData ShadowDamage;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, ShadowDamage)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData DivineDamage;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, DivineDamage)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Armor)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData ParryRate;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, ParryRate)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData DogeRate;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, DogeRate)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData BlockRate;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, BlockRate)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData PhyResist;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, PhyResist)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData ArcaneResist;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, ArcaneResist)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData FrostResist;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, FrostResist)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData FlameResist;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, FlameResist)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData ShadowResist;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, ShadowResist)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData NaturalResist;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, NaturalResist)

	UPROPERTY(BlueprintReadOnly, Category = "BaseAttributes")
	FGameplayAttributeData DivineResist;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, DivineResist)

protected:

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHeath);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHeath);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	UFUNCTION()
	virtual void OnRep_Energy(const FGameplayAttributeData& OldEnergy);

	UFUNCTION()
	virtual void OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy);

	UFUNCTION()
	virtual void OnRep_Rage(const FGameplayAttributeData& OldRage);

	UFUNCTION()
	virtual void OnRep_MaxRage(const FGameplayAttributeData& OldMaxRage);

	UFUNCTION()
	virtual void OnRep_CastSpeed(const FGameplayAttributeData& OldCastSpeed);

	UFUNCTION()
	virtual void OnRep_MaxMoveSpeed(const FGameplayAttributeData& OldMaxMoveSpeed);
};
