// Copyright Epic Games, Inc. All Rights Reserved.

#include "MGameServerPrivate.h"

#include "MGameServer.h"
#include "GameServices/MPlayerManager.h"
#include "GameServices/MWorldManager.h"

#define LOCTEXT_NAMESPACE "FMGameServerModule"

DEFINE_LOG_CATEGORY(LogMGameServer);


const FString RedisIp = TEXT("42.192.111.227");
const int32 RedisPort = 6379;
const FString RedisPassword = TEXT("123456");

FMGameServerModule* GGameServerModule = nullptr;

void FMGameServerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FCoreDelegates::OnPreExit.AddRaw(this, &FMGameServerModule::HandleCorePreExit);
	
	RedisClient = MakeUnique<FRedisClient>();

	Server = NewObject<UMGameServer>();
	Server->AddToRoot();// 不会被GC

	GGameServerModule = this;
}

void FMGameServerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// 注意，这里的代码能会有模块依赖问题（所依赖的模块已经被提前卸载）
	// 清理代码最好放在 FMGameServerModule::HandleCorePreExit() 中
	
	// 清空Tick
	if (TickDelegateHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
		TickDelegateHandle.Reset();
	}

	GGameServerModule = nullptr;
}

void FMGameServerModule::HandleCorePreExit()
{
	Shutdown();

	if (Server)
	{
		Server->RemoveFromRoot();// 开启GC
		Server->ConditionalBeginDestroy();
		Server = nullptr;
	}
}

void FMGameServerModule::Start()
{
	bFirstTick = true;
	LastTickTime = FDateTime::UtcNow();
	if (!TickDelegateHandle.IsValid())
	{
		TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &FMGameServerModule::Tick), 0.030);
	}

	// 创建服务器并启动
	//FString ListenIp = FZGameTablesModule::Get().GetGameTables()->GameServiceConfig.ListenIp;
	//int32 ListenPort = FZGameTablesModule::Get().GetGameTables()->GameServiceConfig.ListenPort;
	Server->Start(10086);

	// 初始化数据库并连接
	RedisClient->ConnectToRedis(RedisIp, RedisPort, RedisPassword);

	// 全区服功能模块的初始化
	UMPlayerManager::Get()->Init();
}

void FMGameServerModule::Shutdown()
{
	// 全区服功能模块PreShutdown（依赖网络服务）
	
	// 服务器关停
	if (Server->IsRunning())
	{
		Server->Stop();
	}
	
	// 全区服功能模块Shutdown
	UMPlayerManager::Get()->Shutdown();
	UMWorldManager::Get()->Shutdown();

	// 数据库关停
	RedisClient->DisconnectRedis();

	// 清空Tick
	if (TickDelegateHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
		TickDelegateHandle.Reset();
	}
}

bool FMGameServerModule::IsRunning() const
{
	return Server && Server->IsRunning();
}

void FMGameServerModule::OnFirstTick()
{
	// Todo 初始化一些全局功能模块
}

bool FMGameServerModule::Tick(float)
{
	// Todo 服务器Tick 以及其它需要Tick的全区服功能模块
	const FDateTime Now = FDateTime::UtcNow();
	const float DeltaTime = (Now - LastTickTime).GetTotalSeconds();
	LastTickTime = Now;

	if (bFirstTick)
	{
		bFirstTick = false;
		OnFirstTick();
	}

	{
		Server->Tick(DeltaTime);
	
		// Todo 心跳机制，检查数据库连接，检查客户端连接
		Server->DoAliveCheck(Now);
		Server->DoPrintStats(Now);
	}

	if (Now > NextRedisAliveCheckTime)
	{
		NextRedisAliveCheckTime = Now + FTimespan::FromSeconds(5);
		if (RedisClient)
		{
			if (!RedisClient->ExecCommand("PING"))
			{
				RedisClient->ConnectToRedis(RedisIp, RedisPort, RedisPassword);
			}
		}
	}

	// 全区服功能模块Tick
	const FDateTime LocalNow = FDateTime::Now();
	
	UMPlayerManager::Get()->Tick(DeltaTime);
	
	return true;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMGameServerModule, MGameServerModule)