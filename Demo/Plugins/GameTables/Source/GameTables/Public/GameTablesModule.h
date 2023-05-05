// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "UObject/GCObject.h"

class UGameTables;

class FGameTablesModule : public IModuleInterface, FGCObject
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FGameTablesModule &Get()
	{
		return FModuleManager::LoadModuleChecked<FGameTablesModule>("MGameTables");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("MGameTables");
	}
	
	UGameTables* GetGameTables();
	
private:
	
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	
	
	UGameTables* GameTables = nullptr;
};
