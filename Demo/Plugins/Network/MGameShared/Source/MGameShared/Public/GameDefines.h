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