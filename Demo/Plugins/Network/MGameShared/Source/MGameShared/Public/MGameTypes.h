// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MGameDefine.h"

#include "MGameTypes.generated.h"

#define IMPLEMENT_SERIALIZE_FUNCTION() \
void SerializeToArray(TArray<uint8>& Data) const \
{ \
	FMemoryWriter Writer(Data, false); \
	UScriptStruct* DataType = StaticStruct(); \
	DataType->SerializeTaggedProperties(Writer, (uint8*)this, DataType, nullptr); \
} \
void ParseFromArray(const TArray<uint8>& Data) const \
{ \
	FMemoryReader Reader(Data, false); \
	UScriptStruct* DataType = StaticStruct(); \
	DataType->SerializeTaggedProperties(Reader, (uint8*)this, DataType, nullptr); \
} \

/** 道具数据*/
USTRUCT(BlueprintType) 
struct FMItemData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	int32 UID = 0;
	
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	int32 ID = 0;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	int32 Num = 0;

	// 在背包中的位置
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	int32 PositionX = 0;
	
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	int32 PositionY = 0;

	// 装备位置，如果是装备的话
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	EEquipSlot Slot = EEquipSlot::Unknown;
	
	// 武器属性字段，附魔，质变，配置Id
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	int32 Enchanting = 0;
};

/** 背包数据*/
USTRUCT(BlueprintType) 
struct FMInventoryData
{
	GENERATED_USTRUCT_BODY()

	// 背包里道具
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	TArray<FMItemData> Items;

	// 穿戴装备
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	TArray<FMItemData> Equipments;
};

/** 玩家属性数据*/
USTRUCT(BlueprintType) 
struct FRoleData
{
	GENERATED_USTRUCT_BODY()

	//角色ID
	UPROPERTY()
	uint64 ID;
	
	// 角色名
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	FString RoleName;

	// 性别
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	EGender Gender = EGender::None;

	// 阵营
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	ECamp Camp = ECamp::None;

	// 种族
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	ERace Race = ERace::None;

	// 种族分支
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	int32 RaceBranch = 0;

	// 出身
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	EBirth Birth = EBirth::None;

	// 角色职业
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	ERoleClass Class = ERoleClass::None;

	// 升级 角色属性值 = 职业等级成长 + 天赋 + 装备
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	int32 Level = 1;

	// 背包数据, 装备数据
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	FMInventoryData InventoryData;
	
	// Todo 天赋数据（种族天赋）
	
	// Todo 其它功能点数据：外观数据、副职业（合成）

	// Todo 离线缓存
	
	// Todo Buff保存

	// 角色创建时间
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	int64 CreateDate = 0;
};

/** 用户整体数据*/
USTRUCT(BlueprintType)
struct FMPlayerData
{
	GENERATED_USTRUCT_BODY()

	IMPLEMENT_SERIALIZE_FUNCTION()
	
	// 用户ID
	UPROPERTY()
	uint64 ID;
	
	// 账户名Steam Epic PS4 Switch, etc
	UPROPERTY()
	FString Account;

	// 角色数据
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	TArray<FRoleData> RoleData;

	// 上次在线时间戳
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	int64 LastOnlineTime = 0;
	
	// 账户创建时间
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	int64 CreateDate = 0;
};

/** 创建角色参数*/
USTRUCT(BlueprintType)
struct FCreateRoleParams
{
	GENERATED_USTRUCT_BODY()
	
	// 角色名
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FString RoleName;

	// 性别
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	EGender Gender = EGender::None;

	// 种族
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	ERace Race = ERace::None;
	
	// 种族分支
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	int32 RaceBranch = 0;

	// 出身
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	EBirth Birth = EBirth::None;

	// 职业
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	ERoleClass Class = ERoleClass::None;

	// Todo 外观数据
};

/** 登录节目角色数据预览*/
USTRUCT(BlueprintType)
struct FPreviewRoleData
{
	GENERATED_USTRUCT_BODY()

	// 角色名
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FString RoleName;

	// 性别
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	EGender Gender = EGender::None;

	// 种族
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	ERace Race = ERace::None;
	
	// 种族分支
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	int32 RaceBranch = 0;

	// 出身
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	EBirth Birth = EBirth::None;

	// 职业
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	ERoleClass Class = ERoleClass::None;

	// Todo 外观数据
	// Todo 装扮（备）数据
	// Todo 其它特效
};

/* 关卡/副本进度数据*/
USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_USTRUCT_BODY()

	// 唯一ID
	UPROPERTY()
	uint64 ID;

	// 配置
	UPROPERTY()
	int32 CfgID = 0;

	// 副本进度数据
	UPROPERTY()
	int32 Progress = 0;
	
	// 创建时间
	UPROPERTY()
	int64 CreateTime = 0;
};

/* 关卡/副本服务器缓存数据*/
USTRUCT(BlueprintType)
struct FLevelSaveData
{
	GENERATED_USTRUCT_BODY()

	// 存储->转为TMap
	UPROPERTY()
	TArray<FLevelData> LevelData;
};