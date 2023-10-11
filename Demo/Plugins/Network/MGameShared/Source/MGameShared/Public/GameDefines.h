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
enum class EOpErrorCode : uint8
{
	Ok				UMETA(DisplayName = "正常"),
	NotEnough		UMETA(DisplayName = "数量不足"),
	BadParam		UMETA(DisplayName = "参数非法"),
	
	InValidConfig	UMETA(DisplayName = "无效配置"),
	Unknown			UMETA(DisplayName = "未知错误"),
};

UENUM(BlueprintType)
enum class ELoginGameRetCode : uint8
{
	Ok				UMETA(DisplayName = "Ok"),
	UnKnow			UMETA(DisplayName = "UnKnow"),
	NoRole			UMETA(DisplayName = "NoRole"),
	DuplicateLogin	UMETA(DisplayName = "DuplicateLogin"),
};