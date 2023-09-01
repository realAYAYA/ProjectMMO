#pragma once

#include "CoreMinimal.h"
#include "MGameMessage.generated.h"

// Game Network "Message, Rpc" Defined here

/**
 * 
*/
UENUM(BlueprintType)
enum class ERpcErrorCode : uint8
{
	Ok = 0				UMETA(DisplayName = "正常"),
	Unknown = 1			UMETA(DisplayName = "未知错误"),
	Unimplemented = 2	UMETA(DisplayName = "接口未实现"),
	Timeout = 3			UMETA(DisplayName = "调用超时"),
};

UENUM(BlueprintType)
enum class ELoginGameRetCode : uint8
{
	Ok = 0				UMETA(DisplayName = "正常登陆"),
	Unknown = 1			UMETA(DisplayName = "未知错误"),
	NoRole = 2			UMETA(DisplayName = "没有角色"),
	DuplicateLogin = 3	UMETA(DisplayName = "已经在线"),
};

/** Message*/

/**
 * 登录游戏
*/
USTRUCT(BlueprintType)
struct FLoginGameReq
{
	GENERATED_USTRUCT_BODY();
	
	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FString Account;
	
	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FString Password;
	
	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FString ClientVersion;
	
	//FLoginGameReq();
	//void Reset();
	//bool operator==(const FLoginGameReq& Right) const;
	//bool operator!=(const FLoginGameReq& Right) const;
	
};

USTRUCT(BlueprintType)
struct FLoginGameAck
{
	GENERATED_USTRUCT_BODY();

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	ELoginGameRetCode Ret;

	/**  */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	//FRoleData role_data;

	/** 是否为重连 (即服务器上玩家对象已经存在) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	bool bReConnected;
	
	//FLoginGameAck();
	//void Reset();
	//bool operator==(const FLoginGameAck& Right) const;
	//bool operator!=(const FLoginGameAck& Right) const;
	
};

/** Rpc*/