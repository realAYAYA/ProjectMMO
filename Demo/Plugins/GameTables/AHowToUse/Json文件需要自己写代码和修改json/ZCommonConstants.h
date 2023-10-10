#pragma once

#include "Engine/DataTable.h"
#include "ZCommonConstants.generated.h"


USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZRoleInitAttributeEntry
{
	GENERATED_BODY()

	/** 血量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float Hp;

	/** 蓝量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float Mp;

	/** 物攻 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PhyAtt;

	/** 物防 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PhyDef;

	/** 法攻 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float MagAtt;

	/** 法防 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float MagDef;

	/** 气血恢复 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float HpRecoverPercent;

	/** 真元恢复 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float MpRecoverPercent;

	/** 会心倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float CritCoeff;

	/** 会心格挡 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float CritBlock;

	/** 神识 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float Mind;	
};


USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZPlayerConstantEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float AutoMoveStopTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float AutoMoveWalkTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 LockDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float AttackIntervalTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float AutoHealTriggerRatioHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
   	float AutoHealTriggerRatioMP;

	/** 手动锁定距离S1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerLockDistanceHand;

	/** 自动锁定距离S2 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerLockDistanceAuto;

	/** 锁定距离上限S3 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerLockDistanceMax;

	/** 显示锁定目标箭头距离 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerShowArrow;

	/** 2D 进入缩放物体大小的距离 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float EnterScaleSizeDistance_2D;

	/** 重生倒计时 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerRebornTime;

	/** 每次点击减少倒计时的时间 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerReduceTime;

	/** 减少时间次数上限 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerReduceNumMax;

	/** 脱战时间 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerEscapeTime;

	/** 最大探索时长(单位秒) */
   	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 MaxExploreTime;

	/** 体修攻击距离 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerPhysicAttackDistance;
	
	/** 法修攻击距离 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerMagicAttackDistance;

	/** 传送CD (单位：秒) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerTeleportCooldown;

	/** 切换修炼方向的最小Rank需求 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 SwitchCultivationDirectionMinRank;

	/** 角色空闲多长时间，服务器接手角色控制权 (单位：秒) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float ServerTakeControlIdleSeconds;
	
	/** 角色坐标补正距离 近 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerLocationCorrectionDistanceNear;

	/** 角色坐标补正距离 远 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerLocationCorrectionDistanceFar;

	/** 角色补正速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PlayerCorrectionCostSpeed;

	/** 客户端达到了攻击距离 服务器没有 需要给服务器一个容错距离 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float ServerAttackingCorrectionLocation;

	/** mini Map Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float MiniMapWorldWidth;

	/** mini Map Height */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float MiniMapWorldHeight;

	/** 客户端管理Entity的范围，距离主角的长度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float ActivateEntitiesDistance;

	/** 选中框自动消失时间 (单位：秒) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float SelectBoxAutoDisappearTime;

	/** 干预模式空闲多久，变为自动模式 (单位：秒)  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float InterveneToAutoSeconds;

	/** 手动变为自动模式延迟时间 (单位：秒)  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float SetPauseMoveFunctionTime;
	
};

/** 角色神通相关全局参数配置 */
USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZPlayerAbilityConstantEntry
{
	GENERATED_BODY()
	
	/** 神通系统开启的主修等级限制 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 OpenRank;

	/** 开启第二神通树的修炼等级限制 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 OpenSecondaryRank;

	/** 神通槽位开放修炼等级限制 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	TArray<int32> SlotsUnlockRank;
};


USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZNpcConstantEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float AutoMoveStopTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float AutoMoveWalkTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 LockDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float AttackIntervalTime;

	/** 自动锁定距离S2 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float NpcLockDistanceAuto;

	/** 锁定距离上限S3 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float NpcLockDistanceMax;

	/** 怪物坐标补正距离 近 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float MonsterLocationCorrectionDistanceNear;

	/** 怪物坐标补正距离 远 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float MonsterLocationCorrectionDistanceFar;

	/** 怪物补正速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float MonsterCorrectionCostSpeed;
	
};

USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZSpecialIdConstantEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 MoneyItemId;

	/** 机缘道具ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 GoldItemId;

	/** 天机令道具ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 DeluxeShopRefreshItemId;

	/** 神通要诀道具ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 AbilityActivePointItemId;

	/** 神通心得道具ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 AbilityUpgradePointItemId;

	/** 功法点道具ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 KungfuPointItemId;

	/** 天机石道具ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 TreasureTokenItemId;
};


USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZBreathingExerciseConfig
{
	GENERATED_BODY()

	/** 基础灵气 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float BasicLingQi;
	
	/** 吐纳速度（单位：秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float Speed; 

	/** 取消阈值 (例：0.5 代表 50%) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float CancelPct; 

	/** 高级吐纳开始阈值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float HighMinPct; 	
	
	/** 完美吐纳开始阈值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PerfectMinPct; 

	/** 完美吐纳结束阈值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float PerfectMaxPct; 

	/** 2倍吐纳权重 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IdleZ")
	int32 Rate2;

	/** 5倍吐纳权重 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IdleZ")
	int32 Rate5;

	/** 10倍吐纳权重 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IdleZ")
	int32 Rate10;
	
};

/** 雷劫境界配置 */
USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZThunderTestDegreeConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 Degree;

	/** 基础伤害 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float Val;	
};


/** 雷劫 */
USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZThunderTestConfig
{
	GENERATED_BODY()

	/** 雷劫系数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	TArray<float> DamageCoef;

	/** 基础伤害 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	TArray<FZThunderTestDegreeConfig> DamageBase;
	
};

/** 战斗模式 */
USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZFightModeConfig
{
	GENERATED_BODY()

	/** `全体模式`所需等级 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 AllMode_RequireRank = 0;

	/** `全体模式`切换到`和平模式`所需未攻击时长(单位：秒) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float AllMode2PeaceMode_NeedSeconds = 0;
	
};

/** 背包 */
USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZInventoryConfig
{
	GENERATED_BODY()

	/** 初始背包空间 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 InitSpace = 0;

	/** 境界等级提升后扩容背包空间 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 StageUpAddSpace = 0;

	/** 背包已满默认发邮件ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 FullMailId = 0;

	/** 不进包的道具 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	TArray<int32> NotInBagType;
};


USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZAlchemyConfig 
{
	GENERATED_BODY()

	/** 单次炼丹耗时（单位：秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float EachRefiningSeconds;

	/** 触发大保底阈值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 BigChanceValue;

	/** 触发大保底阈值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 SmallChanceValue;

	/** 每日炼丹次数上限 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 MaxDailyCount;	
};

USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZForgeConfig 
{
	GENERATED_BODY()

	/** 单次炼丹耗时（单位：秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	float EachRefiningSeconds;

	/** 触发大保底阈值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 BigChanceValue;

	/** 触发大保底阈值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 SmallChanceValue;
	
	/** 每日炼器次数上限 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 MaxDailyCount;

	/** 每日使用辅材次数上限 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 MaxDailyExtraMaterialsUseCount;

	/** 装备还原的机缘单价 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 RevertCostGold;	
	
};


USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZLeaderboardConfig 
{
	GENERATED_BODY()

	/** 玩家进榜等级要求（仅策划可见） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 EnterRank;
	
	/** 功能入口出现等级要求 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 ShowRank;

	/** 功能解锁等级要求 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 UnlockRank;

	/** 排行榜最大人数显示 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 MaxListNum;

	/** 排行榜最大实际参与排序人数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 MaxListSortNum;

	/** 每日点赞次数上限 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 MaxClickLikeNum;

	/** 点赞奖励道具ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 ItemID;

	/** 点赞奖励道具数量下限 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 MinItemNum;

	/** 点赞奖励道具数量阈值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	int32 MaxItemNum;
};

USTRUCT(BlueprintType)
struct ZGAMETABLES_API FZCommonConstantsConfig
{
	GENERATED_BODY()

	/** 新角色初始属性 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	FZRoleInitAttributeEntry RoleInitAttribute; 

	/** 玩家常量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	FZPlayerConstantEntry Player;
	
	/** 玩家神通相关常量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	FZPlayerAbilityConstantEntry PlayerAbility;
	
	/** NPC常量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	FZNpcConstantEntry Npc;

	/** 特殊ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
   	FZSpecialIdConstantEntry SpecialId;

	/** 吐纳 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	FZBreathingExerciseConfig  BreathingExercise;

	/** 雷劫 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	FZThunderTestConfig ThunderTest;

	/** 炼丹 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	FZAlchemyConfig Alchemy;

	/** 炼器 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	FZForgeConfig Forge;	
	
	/** 排行榜 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	FZLeaderboardConfig Leaderboard;

	/** 战斗模式 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	FZFightModeConfig FightMode;

	/** 背包 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IdleZ")
	FZInventoryConfig Inventory; 
	
};

