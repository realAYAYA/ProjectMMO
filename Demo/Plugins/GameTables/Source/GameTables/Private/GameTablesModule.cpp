#include "GameTablesModule.h"

#include "GameTables.h"

void FGameTablesModule::StartupModule()
{
}

void FGameTablesModule::ShutdownModule()
{
}

UGameTables* FGameTablesModule::GetGameTables()
{
	if (!GameTables)
	{
		GameTables = NewObject<UGameTables>();
		GameTables->AddToRoot();
		GameTables->Init();
	}

	return GameTables;
}

void FGameTablesModule::AddReferencedObjects(FReferenceCollector& Collector)
{
	if (GameTables)
	{
		Collector.AddReferencedObject(GameTables);
	}
}