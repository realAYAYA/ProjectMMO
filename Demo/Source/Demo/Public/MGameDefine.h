#pragma once

#include "CoreMinimal.h"

/**
 * Game Network "Message, Rpc" Defined here
*/
UENUM(BlueprintType)
enum class ERpcErrorCode : uint8
{
	Ok				UMETA(DisplayName = "正常"),
	NotEnough		UMETA(DisplayName = "数量不足"),
	IllegalParam	UMETA(DisplayName = "参数非法"),
	
	NeedReq			UMETA(DisplayName = "需要重新请求"),
	InValidConfig	UMETA(DisplayName = "无效配置"),
	Unknown			UMETA(DisplayName = "未知错误"),
};

UENUM(BlueprintType)
enum class ELoginCode : uint8
{
	Ok				UMETA(DisplayName = "正常登陆"),
	DuplicateLogin 	UMETA(DisplayName = "已经在线"),
	Unknown			UMETA(DisplayName = "未知错误"),
};

/** Rpc*/

UENUM(BlueprintType)
enum class EGender : uint8
{
	None			UMETA(DisplayName = "无"),
	Male 			UMETA(DisplayName = "男"),
	Female			UMETA(DisplayName = "女"),
};

// 出身
UENUM(BlueprintType)
enum class EBirth : uint8
{
	None			UMETA(DisplayName = "None"),
	Student 		UMETA(DisplayName = "男"),
	Female			UMETA(DisplayName = "女"),
};

UENUM(BlueprintType)
enum class ERoleClass : uint8
{
	None		UMETA(Displayname = "None"),
	Warrior		UMETA(Displayname = "Warrior"),
	Mage		UMETA(Displayname = "Mage"),
	Warlock		UMETA(Displayname = "Warlock"),
	Ranger		UMETA(Displayname = "Ranger"),
	Druid		UMETA(Displayname = "Druid"),
	Paladin		UMETA(Displayname = "Paladin"),
	Priest		UMETA(Displayname = "Priest"),
};

UENUM(BlueprintType)
enum class ECamp : uint8
{
	None		UMETA(Displayname = "None"),
	Alliance 	UMETA(Displayname = "联盟"),
	Horde		UMETA(Displayname = "部落"),
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	None		UMETA(Displayname = "None"),
	Dropped		UMETA(Displayname = "Dropped"),
	Equipped	UMETA(Displayname = "Equipped"),
};

UENUM(BlueprintType)
enum class ERace : uint8
{
	None		UMETA(Displayname = "None"),
	Orc			UMETA(Displayname = "Orc"),
	Dwarf		UMETA(Displayname = "Orc"),
	Human		UMETA(Displayname = "Human"),
	Elven		UMETA(Displayname = "Elven"),
	Furry		UMETA(Displayname = "Furry"),
	Undead		UMETA(Displayname = "Undead"),
	Demon		UMETA(Displayname = "Demon"),
	Dragon		UMETA(Displayname = "Demon"),
};

UENUM(BlueprintType)
enum class EHumanBranch : uint8
{
	None		UMETA(Displayname = "None"),
};

UENUM(BlueprintType)
enum class EOrcBranch : uint8
{
	None		UMETA(Displayname = "None"),
};

UENUM(BlueprintType)
enum class EDwarfBranch : uint8
{
	None		UMETA(Displayname = "None"),
};

UENUM(BlueprintType)
enum class EElvenBranch : uint8
{
	None		UMETA(Displayname = "None"),
};

UENUM(BlueprintType)
enum class EFurryBranch : uint8
{
	None		UMETA(Displayname = "None"),
};

UENUM(BlueprintType)
enum class EUndeadBranch : uint8
{
	None		UMETA(Displayname = "None"),
};

UENUM(BlueprintType)
enum class EDemonBranch : uint8
{
	None		UMETA(Displayname = "None"),
};

UENUM(BlueprintType)
enum class EDragonBranch : uint8
{
	None		UMETA(Displayname = "None"),
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Trash				UMETA(Displayname = "Trash"),
	Material			UMETA(Displayname = "Material"),
	Equipment			UMETA(Displayname = "Equipment"),
	Mission				UMETA(Displayname = "Mission"),
	Potion				UMETA(Displayname = "Potion"),
	Scroll				UMETA(Displayname = "Scroll"),
	Book				UMETA(Displayname = "Book"),
	Ammunition			UMETA(Displayname = "Ammunition"),
};

UENUM(BlueprintType)
enum class EEquipSlot : uint8
{
	Unknown				UMETA(Displayname = "Unknown"),
	Head				UMETA(Displayname = "Head"),
	Shoulder			UMETA(Displayname = "Shoulder"),
	Cloak				UMETA(Displayname = "Cloak"),
	Armor				UMETA(Displayname = "Armor"),
	Gloves				UMETA(Displayname = "Gloves"),
	Belt				UMETA(Displayname = "Belt"),
	Shoes				UMETA(Displayname = "Shoes"),
	
	Neck				UMETA(Displayname = "Neck"),
	Ring1				UMETA(Displayname = "Ring1"),
	Ring2				UMETA(Displayname = "Ring2"),

	Weapon1				UMETA(Displayname = "Weapon1"),
	Weapon2				UMETA(Displayname = "Weapon2"),
	RangedWeapon		UMETA(Displayname = "RangedWeapon"),
	//护腕、两饰品不要了；裤子改为纯装饰
	
	// 内衣只作装扮
	Cloth1				UMETA(Displayname = "TopCloth"),
	Cloth2				UMETA(Displayname = "UpperCloth"),
	Pants				UMETA(Displayname = "Pants"),
	InDoorShoes			UMETA(Displayname = "InDoorShoes"),

	MaxNum
};