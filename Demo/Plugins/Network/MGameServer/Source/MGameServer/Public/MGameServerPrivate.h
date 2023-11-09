// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "RedisClient.h"
#include "RedisClient.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMGameServer, Log, All);

class UMGameServer;

class MGAMESERVER_API FMGameServerModule : public IModuleInterface
{
	
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void Start();
	void Shutdown();
	bool IsRunning() const;

	TUniquePtr<FRedisClient> RedisClient;

private:

	void OnFirstTick();
	
	bool Tick(float);

	//void OnDailyRefresh();
	//void OnWeeklyRefresh();
	
	UMGameServer* Server = nullptr;
	
	bool bFirstTick = true;
	FTSTicker::FDelegateHandle TickDelegateHandle;

	FDateTime LastTickTime{0};
	FDateTime NextRedisAliveCheckTime{0};

	FDateTime NextDailyRefreshTime{0};
};

extern FMGameServerModule* MGameServerModule;