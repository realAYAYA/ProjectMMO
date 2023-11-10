#include "MPlayer.h"

#include "MGameSession.h"
#include "RedisOp.h"

UMPlayer::~UMPlayer()
{
}

bool UMPlayer::Init(const uint64 InPlayerId, const FString& InAccount)
{
	PlayerId = InPlayerId;
	Account = InAccount;
	return true;
}

void UMPlayer::Cleanup()
{
}

void UMPlayer::Online(UMGameSession* InSession)
{
	Session = InSession;
}

void UMPlayer::Offline(const UMGameSession* InSession)
{
	check(Session == InSession);

	//UE_LOG()

	Session = nullptr;

	Data.LastOnlineTime = FDateTime::UtcNow().GetTicks();  // 更新离线时间
	
	//MarkNeedSave(true);
}

UMGameSession* UMPlayer::GetSession() const
{
	return Session;
}

FGuid UMPlayer::GetConnId() const
{
	if (Session)
		return Session->ID;
	else
	{
		return FGuid();
	}
}

void UMPlayer::SendToMe(const FGameMessage& InMessage) const
{
	if (!Session)
		return;
		
	Session->Send(InMessage);
}

bool UMPlayer::IsOnline() const
{
	return Session != nullptr;
}

bool UMPlayer::Load()
{
	FRedisOp::LoadPlayerData(GetPlayerId(), &Data);

	// 没有数据就当作新号初始化
	if (Data.ID == 0)
	{
		Data.Account = Account;
		Data.ID = PlayerId;
		Data.CreateDate = FDateTime::Now().GetTicks();
		
		return true;
	}

	// Todo Load功能模块数据
	
	return true;
}

void UMPlayer::Save()
{
	UpdateLastWorldInfo();//更新最后的关卡数据
	
	// Todo 功能模块SaveData

	FRedisOp::SavePlayerData(GetPlayerId(), Data);
}

void UMPlayer::SetCurWorldId(uint64 InWorldId)
{
}

void UMPlayer::EnterWorld(const FGuid& WorldID) const
{
	
}

uint64 UMPlayer::GetCurrentWorldID() const
{
	return 0;
}

uint32 UMPlayer::GetCurrentWorldCfgID() const
{
	return 0;
}

UMWorld* UMPlayer::GetCurrentWorld() const
{
	return nullptr;
}

void UMPlayer::UpdateLastWorldInfo()
{
}

void UMPlayer::OnOnline()
{
}

void UMPlayer::OnOffline()
{
}
