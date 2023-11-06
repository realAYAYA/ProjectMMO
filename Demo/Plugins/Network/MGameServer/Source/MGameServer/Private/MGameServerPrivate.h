// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMGameServer, Log, All);

class FMGameServerModule : public IModuleInterface
{
	
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void Start();
	void Shutdown();
	bool IsRunning() const;

	// RedisClient;

private:

	bool Tick(float);

	//void OnDailyRefresh();
	//void OnWeeklyRefresh();

	//Server;
	//TMap<> Sessions;
	//TMap<> Levels;
	
	bool bFirstTick = true;

	FDateTime LastTickTime{0};
	FDateTime NextSessionAliveCheckTime{0};
	FDateTime NextRedisAliveCheckTime{0};

	FDateTime NextDailyRefreshTime{0};
};

extern FMGameServerModule* MGameServerModule;