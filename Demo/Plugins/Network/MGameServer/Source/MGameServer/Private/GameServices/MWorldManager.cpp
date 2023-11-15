#include "MWorldManager.h"

#include "MWorld.h"

M_UOBJECT_SINGLETON_IMPL(UMWorldManager)

UMWorldManager::~UMWorldManager()
{
}

void UMWorldManager::Init()
{
	LoadData();

	// Todo
	// 创建固定的几个大世界公共区域
	// 管理玩家副本
}

void UMWorldManager::Shutdown()
{
	SaveData();

	if (MainWorld)
		MainWorld->Offline(nullptr);
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
	MainWorld = World;
	
	return World;
}

UMWorld* UMWorldManager::CreateDungeon(const UMPlayer* Player)
{
	return nullptr;
}
