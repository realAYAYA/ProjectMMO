// Fill out your copyright notice in the Description page of Project Settings.


#include "MGACrouch.h"

#include "Characters/MCharacter.h"

UMGACrouch::UMGACrouch()
{
	// 选择技能网络同步的执行方案：本地预测、仅本地执行、服务器先行、仅服务器
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// 技能是否进行实例化，方便存储并同步，大概类似于wow中保存身上的buff
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
}

bool UMGACrouch::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;

	const AMCharacter* Character = CastChecked<AMCharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	return Character->CanCrouch();
}

void UMGACrouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthorityOrPredictionKey(OwnerInfo, &ActivationInfo))
		return;

	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);

	AMCharacter* Character = CastChecked<AMCharacter>(OwnerInfo->AvatarActor.Get());
	Character->Crouch();
}

void UMGACrouch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AMCharacter* Character = CastChecked<AMCharacter>(ActorInfo->AvatarActor.Get());
	Character->UnCrouch();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
