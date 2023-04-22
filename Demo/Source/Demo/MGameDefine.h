#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	None	UMETA(Displayname = "None"),
	Dropped	UMETA(Displayname = "Dropped"),
	Equipped	UMETA(Displayname = "Equipped"),
};