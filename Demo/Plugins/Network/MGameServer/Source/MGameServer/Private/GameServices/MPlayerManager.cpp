#include "MPlayerManager.h"

#include "MPlayer.h"

M_UOBJECT_SINGLETON_IMPL(UMPlayerManager)

UMPlayerManager::~UMPlayerManager()
{
}

void UMPlayerManager::Init()
{
	AllEntities.Empty(2048);
}

void UMPlayerManager::Shutdown()
{
	Cleanup();
}

void UMPlayerManager::Cleanup()
{
	Foreach([this](UMPlayer* InRole) -> bool
	{
		DeletePlayer(InRole);
		return true;
	});

	ProcessJunk();	
}

void UMPlayerManager::Tick(float DeltaTime)
{
	// Todo 玩家延迟离线
	const FDateTime Now = FDateTime::UtcNow();
	if ((Now - LastProcessTime).GetTotalSeconds() > 3)
	{
		ProcessJunk();

		LastProcessTime = Now;
	}
}

UMPlayer* UMPlayerManager::GetByPlayerID(const uint64 Id)
{
	if (const auto Ret = IndexEntities.Find(Id))
		return *Ret;
	
	return nullptr;
}

UMPlayer* UMPlayerManager::CreatePlayer(const uint64 InPlayerId, const FString& InAccount)
{
	if (GetByPlayerID(InPlayerId))
	{
		return nullptr;
	}

	UMPlayer* Player = NewObject<UMPlayer>(this);
	if (!Player->Init(InPlayerId, InAccount))
	{
		Player->ConditionalBeginDestroy();
		return nullptr;
	}
	
	if (!AddPlayer(Player))
		return nullptr;
	
	return Player;
}

void UMPlayerManager::DeletePlayer(UMPlayer* InPlayer)
{
	if (!InPlayer)
		return;

	/*if (InPlayer->IsRecycle())
		return;

	InPlayer->MarkRecycle();*/
	
	const uint64 ID = InPlayer->GetPlayerID();
	const auto Ret = IndexEntities.Find(ID);
	if (Ret && *Ret == InPlayer)
	{
		*Ret = nullptr;		

		for (int32 i = 0; i < AllEntities.Num(); ++i)
		{
			if (AllEntities[i] == InPlayer)
			{
				AllEntities[i] = nullptr;
				break;
			}
		}

		Junks.Emplace(InPlayer);
	}
}

void UMPlayerManager::DeletePlayerById(const uint64 InPlayerId)
{
	auto* Player = GetByPlayerID(InPlayerId);
	if (!Player)
		return;
	
	DeletePlayer(Player);
}

void UMPlayerManager::Foreach(const TFunction<bool(UMPlayer*)>& InFunc)
{
	for (const auto& Player : AllEntities)
	{
		if (Player)
		{
			if (!InFunc(Player))
				return;
		}
	}
}

void UMPlayerManager::SendToAll(const FGameMessage& InMessage)
{
	Foreach([this, InMessage](const UMPlayer* Player) -> bool
	{
		Player->SendToMe(InMessage);
		return true;
	});
}

bool UMPlayerManager::AddPlayer(UMPlayer* InPlayer)
{
	const int64 ID = InPlayer->GetPlayerID();
	
	if (UMPlayer** Ret = IndexEntities.Find(ID))
	{
		return false;  // 该ID的角色已存在
	}
	else
	{
		IndexEntities.Add(ID, InPlayer);
	}
	
	{
		// 试着查询一个空位置
		int32 EmptyIdx = INDEX_NONE;
		for (int32 i = 0; i < AllEntities.Num(); ++i)
		{
			const UMPlayer* Ptr = AllEntities[i];
			if (EmptyIdx == INDEX_NONE && Ptr == nullptr)
			{
				EmptyIdx = i;
				break;
			}
		}
		
		if (EmptyIdx == INDEX_NONE)
		{
			EmptyIdx = AllEntities.AddUninitialized();
		}
		
		AllEntities[EmptyIdx] = InPlayer;
	}

	return true;
}

void UMPlayerManager::ProcessJunk()
{
	for (UMPlayer* Player : Junks)
	{
		IndexEntities.Remove(Player->GetPlayerID());
		Player->ConditionalBeginDestroy();
	}
	
	Junks.Empty();
}
