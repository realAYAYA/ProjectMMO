#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "MRpc/Public/NetFwd.h"
#include "GameRpcClient.generated.h"

class URpcManager;

class UGameRpcClient final : public UObject
{
	
public:

	void Setup(URpcManager* InManager, const FConnectionPtr& InConn);
	
};
