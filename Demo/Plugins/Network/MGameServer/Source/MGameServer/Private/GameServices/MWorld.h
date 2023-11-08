#pragma once

#include "CoreMinimal.h"

#include "MWorld.generated.h"

class UMGameSession;

UCLASS()
class UMWorld : public UObject
{
	GENERATED_BODY()
	
public:
	
	virtual ~UMWorld() override;

private:

	const uint64 UserId = 0;
	// DSProcessHandle;

	UPROPERTY()
	UMGameSession* Session = nullptr;

	
};