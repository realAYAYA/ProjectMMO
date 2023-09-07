// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

/** Declares a log category for this module. */
DECLARE_LOG_CATEGORY_EXTERN(LogRedisClient, Log, All);

class FRedisClientModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
};
