// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_DirectSkill.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemLog.h"
#include "MBlueprintLibrary.h"

#include "Characters/MCharacter.h"
#include "GameplayAbilitySystem/AttributeSets/MAttributeSet.h"
#include "GameplayAbilitySystem/Components/MAbilitySystemComponent.h"
#include "GameplayAbilitySystem/GameplayEffects/MGameplayEffect.h"
#include "Kismet/KismetMathLibrary.h"

bool UGA_DirectSkill::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	const AMCharacter* Character = Cast<AMCharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
		return false;
	
	if (!Cast<UMAbilitySystemComponent>(Character->GetAbilitySystemComponent()))
	{
		Character->OnAbilityFailed.Broadcast(EActivateFailCode::BadMemory);
		return false;
	}

	if (CanActivateCondition(*ActorInfo) != EActivateFailCode::Success)
		return false;
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_DirectSkill::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
	
	// Note: 确保在进入该函数之前，一切指针变量所指向的内存是安全的
	
	AMCharacter* Caster = Cast<AMCharacter>(OwnerInfo->AvatarActor);
	
	if (TargetType == ETargetType::SelfOnly)
	{
		Target = Caster;
	}
	else if (TargetType == ETargetType::SelfOrFriendly && !Caster->CurrentTarget)
	{
		Target = Caster;
	}
	else
	{
		Target = Cast<AMCharacter>(OwnerInfo->AvatarActor)->CurrentTarget;
	}
	
	UAbilitySystemComponent* ASC = OwnerInfo->AbilitySystemComponent.Get();
	
	// 对目标施加效果
	UAbilitySystemComponent* TargetComponent = Target->GetAbilitySystemComponent();
	for (const TSubclassOf<UMGameplayEffect>& Effect : EffectsToTargetOnStart)
	{
		if (!Effect.Get())
			continue;

		const FGameplayEffectContextHandle TargetEffectContext = ASC->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Effect, Level, TargetEffectContext);
		if (SpecHandle.IsValid())
		{
			const FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetComponent);
			if (!ActiveGEHandle.WasSuccessfullyApplied())
				ABILITY_LOG(Log, TEXT("Ability %s faild to apply Effect to Target %s"), *GetName(), *GetNameSafe(Effect));
		}
	}
}

void UGA_DirectSkill::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if (!IsInstantiated())
		return;

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	// 对目标施加效果
	UAbilitySystemComponent* TargetComponent = Target->GetAbilitySystemComponent();
	for (const TSubclassOf<UMGameplayEffect>& Effect : EffectsToTargetOnEnd)
	{
		if (!Effect.Get())
			continue;

		const FGameplayEffectContextHandle TargetEffectContext = ASC->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Effect, Level, TargetEffectContext);
		if (SpecHandle.IsValid())
		{
			const FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetComponent);
			if (!ActiveGEHandle.WasSuccessfullyApplied())
				ABILITY_LOG(Log, TEXT("Ability %s faild to apply Effect to Target %s"), *GetName(), *GetNameSafe(Effect));
		}
	}
	
	
}

EActivateFailCode UGA_DirectSkill::CanActivateCondition(const FGameplayAbilityActorInfo& ActorInfo) const
{
	const AMCharacter* Caster = Cast<AMCharacter>(ActorInfo.AvatarActor.Get());
	const AMCharacter* CurrentTarget = Caster->CurrentTarget;

	// 根据目标类型检查是否可以释放技能
	switch (TargetType)
	{
	default:break;
	case ETargetType::FriendlyOnly:
		{
			// 目标如果不存在或不是友军
			if (!CurrentTarget || Caster == CurrentTarget || !UMBlueprintLibrary::IsFriendly(Caster, CurrentTarget))
				return EActivateFailCode::InValidTarget;
		}
	case ETargetType::SelfOrFriendly:
		{
			// 如果目标是敌对的则选择自己
			if (!UMBlueprintLibrary::IsFriendly(Caster, CurrentTarget))
				return EActivateFailCode::InValidTarget;
		}
	case ETargetType::HostileOnly:
		{
			if (UMBlueprintLibrary::IsFriendly(Caster, CurrentTarget))
				return EActivateFailCode::InValidTarget;
			
			CurrentTarget = Caster->CurrentTarget;
		}
	}
	
	if (!CurrentTarget || !CurrentTarget->GetAbilitySystemComponent())
	{
		return EActivateFailCode::NoTarget;
	}
	
	// 消耗条件不足
	if (Caster->GetAttributeSet()->Mana.GetCurrentValue() < Mana)
	{
		return EActivateFailCode::NoMana;
	}
	if (Caster->GetAttributeSet()->Energy.GetCurrentValue() < Energy)
	{
		return EActivateFailCode::NoEnergy;
	}
	if (Caster->GetAttributeSet()->Rage.GetCurrentValue() < Rage)
	{
		return EActivateFailCode::NoRage;
	}
	
	// Out of range
	const float Distance = (Caster->GetActorLocation() - CurrentTarget->GetActorLocation()).Length();
	if (Distance > Range)
	{
		return EActivateFailCode::OutOfRange;
	}

	// 目标过近
	if (Distance < MinRange)
	{
		return EActivateFailCode::TooClose;
	}
	
	// 如果是目标敌对，需要面向对方，友方增益buff则不需要
	if (TargetType == ETargetType::HostileOnly)
	{
		const FVector Dir = UKismetMathLibrary::Normal(CurrentTarget->GetActorLocation() - Caster->GetActorLocation(), 0.0001);
		if (UKismetMathLibrary::Dot_VectorVector(Dir, Caster->GetActorForwardVector()) < 0.5f)
		{
			return EActivateFailCode::NoToward;
		}
	}
	
	// Todo 不在视野中
	
	return EActivateFailCode::Success;
}