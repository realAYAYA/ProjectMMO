// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MGameDefine.h"

#include "MGameTypes.generated.h"

/** 游戏中Buff数据*/
USTRUCT(BlueprintType) 
struct FGameBuffData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int32 ID = 0;

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int32 RemainingTime = 0;

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int32 Overflow = 0;
};

/** 玩家属性数据*/
USTRUCT(BlueprintType) 
struct FCharacterAttribute
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int32 Level = 0;

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int32 Health = 0;

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int32 Mana = 0;

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int32 Rage = 0;

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int32 Energy = 0;
};

/** 游戏玩家角色数据*/
USTRUCT(BlueprintType) 
struct FGamePlayerData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int64 UserID = 0;

	// 升级数据
	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int32 LevelData = 0;

	// 阵营
	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	ECamp Camp;

	// 角色职业
	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	ECharacterClass Class;
	
	// Todo 装备数据、背包数据、天赋数据（种族天赋）、外观数据
	// Todo 其它功能点数据：军衔、副职业

	// Todo 离线缓存
	// 角色属性值
	// Todo Buff保存
	
};

/** 用户整体数据*/
USTRUCT(BlueprintType)
struct FGameUserData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int64 UserID = 0;

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	FGamePlayerData PlayerData;
	
};


USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TArray<TSubclassOf<class UGameplayEffect>> Effects;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> Abilities;
};

UCLASS()
class UItemData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = ProjectSS)
	int32 ID = 0;
	
};

UCLASS()
class UGearData : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = ProjectSS)
	TSubclassOf<UGameplayEffect> DamageEffect;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = ProjectSS)
	TSoftObjectPtr<class USkeletalMesh> SkeletalMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = ProjectSS)
	TSoftObjectPtr<class UStaticMesh> StaticMesh;
};