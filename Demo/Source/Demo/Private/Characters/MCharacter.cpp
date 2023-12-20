// Fill out your copyright notice in the Description page of Project Settings.


#include "MCharacter.h"
#include "MCharacterDataAsset.h"
#include "MPlayerState.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySystem/Components/MAbilitySystemComponent.h"
#include "GameplayAbilitySystem/AttributeSets/MAttributeSet.h"
#include "Components/MCharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMCharacter::AMCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMCharacterMovementComponent>(CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -89.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	// Create a SpringArmComponent
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->SetRelativeLocation(FVector(0.f, 0.f, 8.5f));
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	// Create a CameraComponent	
	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCameraComponent->SetupAttachment(SpringArmComponent);

	// Gameplay Ability System
	AbilitySystemComponent = CreateDefaultSubobject<UMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UMAttributeSet>(TEXT("AttributeSet"));
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxMoveSpeedAttribute()).AddUObject(this, &AMCharacter::OnMaxMovementSpeedChanged);
	
}

void AMCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// PossessedBy()只在服务器执行
	InitReplicatedData();
}

void AMCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

#if !UE_SERVER
	// 如果不是以DS编译，而是以Listening Server启动方式时，OnRep不会在主机上调用，需要手动触发
#endif
	
}

void AMCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(CharacterDataAsset))
		CharacterData = CharacterDataAsset->CharacterData;
}

AMPlayerState* AMCharacter::GetMPlayerState() const
{
	return Cast<AMPlayerState>(GetPlayerState());
}

void AMCharacter::InitReplicatedData()
{
	const AMPlayerState* PS = GetMPlayerState();
	if (PS && PS->IsOnline())
	{
		const FRoleData& Data = PS->GetRoleData();
		RoleName = FName(Data.RoleName);
		RoleClass = Data.Class;
		Race = Data.Race;

		GiveAbilities();
		ApplyStartupEffects();
		UpdateModel();
	}
}

void AMCharacter::UpdateModel()
{
	// Todo 从角色各个功能模块中计算外观
	//const AMPlayerState* PS = GetMPlayerState();
}

// Called when the game starts or when spawned
void AMCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMCharacter::SetCurrentTarget_Implementation(AMCharacter* NewTarget)
{
	CurrentTarget = NewTarget;

#if !UE_SERVER
	// 如果不是以DS编译，而是以Listening Server启动方式时，OnRep不会在主机上调用，需要手动触发
	if (UKismetSystemLibrary::IsServer(this) && !UKismetSystemLibrary::IsDedicatedServer(this))
	{
		OnCurrentTargetChanged.Broadcast(CurrentTarget);
	}
#endif
	
}

void AMCharacter::SetRoleName(const FString& InName)
{
	RoleName = FName(*InName);

#if !UE_SERVER
	// 如果不是以DS编译，而是以Listening Server启动方式时，OnRep不会在主机上调用，需要手动触发
	if (UKismetSystemLibrary::IsServer(this) && !UKismetSystemLibrary::IsDedicatedServer(this))
	{
		OnRoleNameChanged.Broadcast(RoleName);
	}
#endif
}

void AMCharacter::SetRoleCamp(const ECamp& InCamp)
{
	Camp = InCamp;

#if !UE_SERVER
	// 如果不是以DS编译，而是以Listening Server启动方式时，OnRep不会在主机上调用，需要手动触发
	if (UKismetSystemLibrary::IsServer(this) && !UKismetSystemLibrary::IsDedicatedServer(this))
	{
		OnRoleCampChanged.Broadcast(InCamp);
	}
#endif
}

void AMCharacter::SetRoleRace(const ERace& InRace)
{
	Race = InRace;

#if !UE_SERVER
	// 如果不是以DS编译，而是以Listening Server启动方式时，OnRep不会在主机上调用，需要手动触发
	if (UKismetSystemLibrary::IsServer(this) && !UKismetSystemLibrary::IsDedicatedServer(this))
	{
		OnRoleRaceChanged.Broadcast(InRace);
	}
#endif
}

void AMCharacter::SetModelData(const FModelData& InData)
{
	ModelData = InData;

#if !UE_SERVER
	// 如果不是以DS编译，而是以Listening Server启动方式时，OnRep不会在主机上调用，需要手动触发
	if (UKismetSystemLibrary::IsServer(this) && !UKismetSystemLibrary::IsDedicatedServer(this))
	{
		OnModelDataChanged.Broadcast();
	}
#endif
}

void AMCharacter::OnRep_RoleName() const
{
	OnRoleNameChanged.Broadcast(RoleName);
}

void AMCharacter::OnRep_RoleCamp() const
{
	OnRoleCampChanged.Broadcast(Camp);
}

void AMCharacter::OnRep_RoleRace() const
{
	OnRoleRaceChanged.Broadcast(Race);
}

void AMCharacter::OnRep_CurrentTarget() const
{
	OnCurrentTargetChanged.Broadcast(CurrentTarget);
}

void AMCharacter::OnRep_ModelData() const
{
	OnModelDataChanged.Broadcast();
}

UAbilitySystemComponent* AMCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AMCharacter::ApplyGameplayEffectToSelf(
	const TSubclassOf<UGameplayEffect> Effect,
	const FGameplayEffectContextHandle& InEffectContext) const
{
	if (!Effect.Get())
		return false;

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, InEffectContext);
	if (SpecHandle.IsValid())
	{
		const FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		return ActiveGEHandle.WasSuccessfullyApplied();
	}

	return false;
}

void AMCharacter::GiveAbilities()
{
	// Todo 根据种族、出身、职业、副职业、天赋获取技能
	
	if (!HasAuthority() || !AbilitySystemComponent)
		return;

	//AbilitySystemComponent->ClearAbility()
	
	for (const auto& AbilityClass : CharacterData.DefaultAbilities)
	{
		if (!AbilityClass)
			continue;
			
		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(AbilityClass);
		AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);
	}

	TArray<TSubclassOf<UGameplayAbility>>* Abilities;
	
	switch (RoleClass)
	{
		default: return;
	case ERoleClass::Warrior: Abilities = &CharacterData.WarriorAbilities;break;
	case ERoleClass::Mage: Abilities = &CharacterData.MageAbilities;break;
	case ERoleClass::Priest: Abilities = &CharacterData.PriestAbilities;break;
	}
	
	for (const auto& AbilityClass : *Abilities)
	{
		if (!AbilityClass)
			continue;
			
		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(AbilityClass);
		AbilitySystemComponent->GiveAbility(GameplayAbilitySpec);
	}
}

void AMCharacter::ApplyStartupEffects()
{
	// Todo 根据上次离线状态初始化身上的buff和属性值
	
	if (!HasAuthority())
		return;

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	for (const auto& Effect : CharacterData.Effects)
	{
		if (ApplyGameplayEffectToSelf(Effect, EffectContextHandle))
		{
			//UE_LOG(LogProjectM, Error, TEXT("%s : %s Failed"), *FString(__FUNCTION__), *FString(Effect->GetName()));
		}
	}
}

void AMCharacter::TryJump(const FInputActionValue& Value)
{
	// 检测是否可以移动
	FGameplayTagContainer Container;
	Container.AddTag(FGameplayTag::RequestGameplayTag(FName("GAS.GE.Limit.Root")));
	Container.AddTag(FGameplayTag::RequestGameplayTag(FName("GAS.GE.Limit.Paralysis")));// 瘫痪类buff

	for (const FGameplayTag& Tag :Container)
	{
		if (GetAbilitySystemComponent()->HasMatchingGameplayTag(Tag))
			return;
	}
	
	AbilitySystemComponent->Jump();
}

void AMCharacter::MoveBeginInternal(const FVector2D& Value)
{
	// 检测是否可以移动
	FGameplayTagContainer Container;
	Container.AddTag(FGameplayTag::RequestGameplayTag(FName("GAS.GE.Limit.Root")));
	Container.AddTag(FGameplayTag::RequestGameplayTag(FName("GAS.GE.Limit.Paralysis")));// 瘫痪类buff

	for (const FGameplayTag& Tag :Container)
	{
		if (GetAbilitySystemComponent()->HasMatchingGameplayTag(Tag))
			return;
	}

	if (Controller != nullptr)
	{
		// add movement
		AddMovementInput(GetActorForwardVector(), FMath::Clamp(Value.Y, -0.5f, 1));
		AddMovementInput(GetActorRightVector(), Value.X);
		
		AbilitySystemComponent->MoveBegin();
	}
}

void AMCharacter::MoveEndInternal(const FVector2D& Value) const
{
	if (Controller != nullptr)
	{
		AbilitySystemComponent->MoveEnd();
	}
}

void AMCharacter::LookInternal(const FVector2D& Value)
{
	// 检测是否可以四周观察
	FGameplayTagContainer Container;
	Container.AddTag(FGameplayTag::RequestGameplayTag(FName("GAS.GE.Limit.Paralysis")));// 瘫痪类buff

	for (const FGameplayTag& Tag :Container)
	{
		if (GetAbilitySystemComponent()->HasMatchingGameplayTag(Tag))
			return;
	}

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Value.X);
		AddControllerPitchInput(Value.Y);
	}
}

void AMCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	AbilitySystemComponent->JumpEnd();
}

void AMCharacter::OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data) const
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void AMCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMCharacter, CharacterData);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	
	SharedParams.RepNotifyCondition = REPNOTIFY_OnChanged;
	DOREPLIFETIME_WITH_PARAMS_FAST(AMCharacter, CurrentTarget, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AMCharacter, ModelData, SharedParams);
	
	DOREPLIFETIME_WITH_PARAMS_FAST(AMCharacter, Camp, SharedParams);

	DOREPLIFETIME_WITH_PARAMS_FAST(AMCharacter, RoleName, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AMCharacter, RoleClass, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AMCharacter, Race, SharedParams);
}
