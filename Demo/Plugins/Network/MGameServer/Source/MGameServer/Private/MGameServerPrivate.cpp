// Copyright Epic Games, Inc. All Rights Reserved.

#include "MGameServerPrivate.h"

#include "MGameServer.h"

#define LOCTEXT_NAMESPACE "FMWebSocketServerModule"

DEFINE_LOG_CATEGORY(LogMGameServer);

void FMGameServerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FMGameServerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// 注意，这里的代码能会有模块依赖问题（所依赖的模块已经被提前卸载）
	// 清理代码最好放在 FMGameServerModule::HandleCorePreExit 中
	
	if (TickDelegateHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
		TickDelegateHandle.Reset();
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
	Server = NewObject<UMGameServer>();

	Server2 = MakeShared<UMGameServer>();
	
	Server->AddToRoot();
	//FString ListenIp = FZGameTablesModule::Get().GetGameTables()->GameServiceConfig.ListenIp;
	//int32 ListenPort = FZGameTablesModule::Get().GetGameTables()->GameServiceConfig.ListenPort;

	// 初始化数据库并连接

	// Todo 各种全区服功能模块的初始化
	
}

void FMGameServerModule::Shutdown()
{
	// 全区服功能模块PreShutdown（依赖网络服务）
	// 服务器关停
	// 全区服功能模块Shutdown

	// 数据库关停

	// 清空Tick
	if (TickDelegateHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
		TickDelegateHandle.Reset();
	}
}

bool FMGameServerModule::IsRunning() const
{
	return false;
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

	

	// Todo 心跳机制，检查数据库连接，检查客户端连接

	const FDateTime LocalNow = FDateTime::Now();
	
	return true;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMGameServerModule, MWebSocketServer)