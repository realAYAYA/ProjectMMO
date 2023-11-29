// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "InputActionValue.h"

#include "MGameCommon.h"
#include "MGameTypes.h"
#include "MCharacter.generated.h"

class USpringArmComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UGameplayAbility;
class UGameplayEffect;

class UMAbilitySystemComponent;
class UMAttributeSet;
class AMPlayerController;
class AMPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoleNameChanged, FName, NewName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoleCampChanged, ECamp, NewCamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoleRaceChanged, ERace, NewRace);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentTargetChanged, AMCharacter*, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityFailed, EActivateFailCode, FailCode);

// 游戏角色基类
UCLASS()
class DEMO_API AMCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	/** Third person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCameraComponent;

	/** Third person camera arm */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

public:
	
	// Sets default values for this character's properties
	AMCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void OnRep_PlayerState() override;

	virtual void PostInitializeComponents() override;

	UCameraComponent* GetThirdPersonCameraComponent() const { return ThirdPersonCameraComponent; }

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	AMPlayerState* GetMPlayerState() const;

	// 加载数据
	void LoadData();

	// 加载外观
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void LoadModel();
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	// Network-Replicate
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	AMCharacter* GetCurrentTarget() const { return CurrentTarget; }
	
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ProjectM")
	void SetCurrentTarget(AMCharacter* NewTarget);

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnCurrentTargetChanged OnCurrentTargetChanged;

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	FName GetRoleName() const { return RoleName; }
	
	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnRoleNameChanged OnRoleNameChanged;

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	ECamp GetRoleCamp() const { return Camp; }
	
	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnRoleCampChanged OnRoleCampChanged;

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	ERace GetRoleRace() const { return Race; }
	
	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnRoleRaceChanged OnRoleRaceChanged;

	UFUNCTION()
	void SetRoleName(const FString& InName);

	UFUNCTION()
	void SetRoleCamp(const ECamp& InCamp);

	UFUNCTION()
	void SetRoleRace(const ERace& InRace);
	
protected:
	
	UFUNCTION()
	void OnRep_RoleName() const;

	UFUNCTION()
	void OnRep_RoleCamp() const;

	UFUNCTION()
	void OnRep_RoleRace() const;

	UFUNCTION()
	void OnRep_CurrentTarget() const;
	
	/*UFUNCTION()
	void OnRep_Appearance() const;*/
	
private:

	/** 当前锁定目标*/
	UPROPERTY(ReplicatedUsing = OnRep_CurrentTarget)
	AMCharacter* CurrentTarget;

	UPROPERTY(ReplicatedUsing = OnRep_RoleName)
	FName RoleName;

	UPROPERTY(ReplicatedUsing = OnRep_RoleCamp)
	ECamp Camp;

	UPROPERTY(ReplicatedUsing = OnRep_RoleRace)
	ERace Race;

	UPROPERTY(Replicated)
	ERoleClass RoleClass;

	// Todo 外观数据，及其同步

	// GAS
	
public:

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	const UMAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	const UMAbilitySystemComponent* GetAsc() const { return AbilitySystemComponent; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMAbilitySystemComponent* AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	bool ApplyGameplayEffectToSelf(
		const TSubclassOf<UGameplayEffect> Effect,
		const FGameplayEffectContextHandle& InEffectContext) const;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnAbilityFailed OnAbilityFailed;

	// 初始化技能系统，职业|天赋|种族|出身|
	UFUNCTION()
	void GiveAbilities();
	
protected:

	UPROPERTY(Replicated)
	FCharacterData CharacterData;// Todo 测试技能配置

	UPROPERTY(EditDefaultsOnly)
	class UMCharacterDataAsset* CharacterDataAsset;
	
	void ApplyStartupEffects();

	//FDelegateHandle MaxMovementSpeedChangedDelegatedHandle;
	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);

private:
	
	UPROPERTY(Transient)
	UMAttributeSet* AttributeSet;

	
	// Character Control

protected:

	/** Called for jump input */
	void TryJump(const FInputActionValue& Value);

	void MoveBeginInternal(const FVector2D& Value);
	void MoveEndInternal(const FVector2D& Value);

	void LookInternal(const FVector2D& Value);

	void JumpBeginInternal(const bool IsHeightJump);
	
	virtual void Landed(const FHitResult& Hit) override;

};
