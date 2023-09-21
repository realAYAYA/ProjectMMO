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
enum class ELoginCode : uint8
{
	Ok = 0				UMETA(DisplayName = "正常登陆"),
	Unknown = 1			UMETA(DisplayName = "未知错误"),
	NoRole = 2			UMETA(DisplayName = "没有角色"),
	DuplicateLogin = 3	UMETA(DisplayName = "已经在线"),
};

/** Rpc*/