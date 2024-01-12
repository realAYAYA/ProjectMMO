#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ERpcMessageOp : uint8
{
	Notify			UMETA(DisplayName = "Notify"),
	Request			UMETA(DisplayName = "Request"),
	Response		UMETA(DisplayName = "Response"),
};

UENUM(BlueprintType)
enum class ERpcErrorCode : uint8
{
	Ok				UMETA(DisplayName = "Ok"),
	UnKnow			UMETA(DisplayName = "UnKnow"),
	Unimplemented	UMETA(DisplayName = "Unimplemented"),
	TimeOut			UMETA(DisplayName = "TimeOut"),
};

UENUM(BlueprintType)
enum class ELoginGameRetCode : uint8
{
	Ok				UMETA(DisplayName = "Ok"),
	UnKnow			UMETA(DisplayName = "UnKnow"),
	NeedUpdate		UMETA(DisplayName = "NoRole"),
	DuplicateLogin	UMETA(DisplayName = "DuplicateLogin"),
};

// -----------------------------------------------------业务枚举---------------------------------------------------------

// 离开世界的理由
UENUM(BlueprintType)
enum class ELeaveWorldReason : uint8
{
	Normal				UMETA(Displayname = "Normal"),
	Disconnected		UMETA(Displayname = "Disconnected"),
	Kicked				UMETA(Displayname = "Kicked"),
	ChangeWorld			UMETA(Displayname = "ChangeWorld"),
};

// ---------------------------------------------------Gameplay枚举-------------------------------------------------------

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
	Student			UMETA(DisplayName = "Student"),
	Farmer 			UMETA(DisplayName = "Farmer"),
	Worker 			UMETA(DisplayName = "Worker"),
	Bureaucrat		UMETA(DisplayName = "Bureaucrat"),
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
	Dwarf		UMETA(Displayname = "Dwarf"),
	Human		UMETA(Displayname = "Human"),
	Elven		UMETA(Displayname = "Elven"),
	Furry		UMETA(Displayname = "Furry"),
	Undead		UMETA(Displayname = "Undead"),
	Demon		UMETA(Displayname = "Demon"),
	Dragon		UMETA(Displayname = "Dragon"),
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

UENUM(BlueprintType)
enum class EDressType : uint8
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

	Cloth1				UMETA(Displayname = "TopCloth"),
	Cloth2				UMETA(Displayname = "UpperCloth"),
	Pants				UMETA(Displayname = "Pants"),
	InDoorShoes			UMETA(Displayname = "InDoorShoes"),

	MaxNum
};