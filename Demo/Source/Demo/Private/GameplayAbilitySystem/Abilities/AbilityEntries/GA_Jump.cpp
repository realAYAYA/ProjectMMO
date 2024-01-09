// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Jump.h"

#include "AbilitySystemComponent.h"

#include "Characters/MCharacter.h"

UGA_Jump::UGA_Jump()
{
	// 选择技能网络同步的执行方案：本地预测、仅本地执行、服务器先行、仅服务器
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// 技能是否进行实例化，方便存储并同步，大概类似于wow中保存身上的buff
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_Jump::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;

	const AMCharacter* Character = CastChecked<AMCharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	return Character->CanJump();
}

void UGA_Jump::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!HasAuthorityOrPredictionKey(OwnerInfo, &ActivationInfo))
		return;

	if (!CommitAbility(Handle, OwnerInfo, ActivationInfo))
		return;

	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);

	AMCharacter* Character = CastChecked<AMCharacter>(OwnerInfo->AvatarActor.Get());
	Character->Jump();
}
