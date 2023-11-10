#include "MWorldManager.h"

#include "MWorld.h"

M_UOBJECT_SINGLETON_IMPL(UMWorldManager)

UMWorldManager::~UMWorldManager()
{
}

void UMWorldManager::Init()
{
	LoadData();
}

void UMWorldManager::Shutdown()
{
	SaveData();
}

void UMWorldManager::Cleanup()
{
}

void UMWorldManager::LoadData()
{
	// Todo 加载玩家副本存档
}

void UMWorldManager::SaveData()
{
	// Todo 储存副本存档
}

bool UMWorldManager::Tick(float DeltaTime)
{
	return false;
}

UMWorld* UMWorldManager::CreateWorld()
{
	/*if (GetByPlayerId(InPlayerId))
	{
		return nullptr;
	}*/

	UMWorld* World = NewObject<UMWorld>(this);
	if (!World->Init())
	{
		World->ConditionalBeginDestroy();
		return nullptr;
	}
	
	/*if (!AddPlayer(Player))
		return nullptr;*/
	
	return World;
}
