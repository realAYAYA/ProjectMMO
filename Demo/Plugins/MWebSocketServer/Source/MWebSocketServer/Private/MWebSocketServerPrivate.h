// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMWebSocketServer, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMWebSocketClient, Log, All);

class FMWebSocketServerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
