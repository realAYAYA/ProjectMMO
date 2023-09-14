// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MGameDefine.h"

#include "MGameTypes.generated.h"

/** 玩家离线后，游戏中角色身上Buff快照数据 */
USTRUCT(BlueprintType) 
struct FMBuffSnapShoot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	int32 ID = 0;

	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	int32 RemainingTime = 0;

	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	int32 Stack = 0;

	// Todo 从玩家AbilityComponent上读取当前所有效果，及其运行数据
};

/** 道具数据*/
USTRUCT(BlueprintType) 
struct FMItemData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	int32 UID = 0;
	
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	int32 ID = 0;

	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	int32 Num = 0;

	// 在背包中的位置
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	int32 PositionX = 0;
	
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	int32 PositionY = 0;

	// 武器属性字段，附魔，质变
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	int32 Enchanting = 0;
	
};

/** 背包数据*/
USTRUCT(BlueprintType) 
struct FMInventoryData
{
	GENERATED_USTRUCT_BODY()

	// 背包里道具
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	TArray<FMItemData> Items;

	// 穿戴装备
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	TArray<FMItemData> Equipments;
};

/** 玩家属性数据*/
USTRUCT(BlueprintType) 
struct FMCharacterData
{
	GENERATED_USTRUCT_BODY()

	// 阵营
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	ECamp Camp;

	// 角色职业
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	ECharacterClass CharacterClass = ECharacterClass::None;

	// 升级 角色属性值 = 职业等级成长 + 天赋 + 装备
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	int32 Level = 1;

	// 背包数据, 装备数据
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	FMInventoryData InventoryData;
	
	// Todo 天赋数据（种族天赋）、外观数据
	
	// Todo 其它功能点数据：军衔、副职业（合成）

	// Todo 离线缓存
	
	// Todo Buff保存

	// 角色创建时间
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	int64 CreateDate = 0;
};

/** 用户整体数据*/
USTRUCT(BlueprintType)
struct FMUserData
{
	GENERATED_USTRUCT_BODY()

	// 用户ID
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	int64 UserID = 0;

	// 用户名
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	FString UserName;

	// 用户角色数据
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	TArray<FMCharacterData> CharacterData;

	// 账户创建时间
	UPROPERTY(BlueprintReadOnly, Category = ProjectM)
	int64 CreateDate = 0;
};

// Todo Delete
USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TArray<TSubclassOf<class UGameplayEffect>> Effects;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> Abilities;
};
