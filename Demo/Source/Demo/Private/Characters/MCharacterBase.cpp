// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilitySystem/Components/MAbilitySystemComponentBase.h"
#include "GameplayAbilitySystem/AttributeSets/MAttributeSetBase.h"

// Sets default values
AMCharacterBase::AMCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Gameplay Ability System
	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponentBase>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UMAttributeSetBase>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AMCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AMCharacterBase::ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle InEffectContext)
{
	if (!Effect.Get())
		return false;

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, InEffectContext);
	if (SpecHandle.IsValid())
	{
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
	}

	return false;
}

void AMCharacterBase::InitializeAttributes()
{
	if (GetLocalRole() == ROLE_Authority && DefaultAttributeSet && AttributeSet)
	{
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		ApplyGameplayEffectToSelf(DefaultAttributeSet, EffectContextHandle);
	}
}

void AMCharacterBase::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (const auto& DefaultAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(DefaultAbility);
		}
	}
}

void AMCharacterBase::ApplyStartupEffects()
{
	if (GetLocalRole() == ROLE_Authority && DefaultAttributeSet && AttributeSet)
	{
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		for (const auto& CharacterEffect : DefaultEffects)
		{
			ApplyGameplayEffectToSelf(CharacterEffect, EffectContextHandle);
		}
		
	}
}

void AMCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AMCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
}

// Called when the game starts or when spawned
void AMCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

