// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

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

	// RedisClient;

private:

	void OnFirstTick();
	
	bool Tick(float);

	//void OnDailyRefresh();
	//void OnWeeklyRefresh();

	UMGameServer* Server = nullptr;
	TSharedPtr<UMGameServer> Server2;
	//TMap<> Sessions;
	//TMap<> Levels;
	
	bool bFirstTick = true;
	FTSTicker::FDelegateHandle TickDelegateHandle;

	FDateTime LastTickTime{0};
	FDateTime NextRedisAliveCheckTime{0};

	FDateTime NextDailyRefreshTime{0};
};

extern FMGameServerModule* MGameServerModule;