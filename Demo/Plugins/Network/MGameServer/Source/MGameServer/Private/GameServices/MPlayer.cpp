#include "MPlayer.h"

#include "MGameSession.h"

UMPlayer::~UMPlayer()
{
}

bool UMPlayer::Init(int64 InUserId, const FString& InAccount, int64 InRoleId)
{
	return false;
}

void UMPlayer::Cleanup()
{
}

void UMPlayer::Online(UMGameSession* InSession)
{
}

void UMPlayer::Offline(UMGameSession* InSession)
{
}

UMGameSession* UMPlayer::GetSession()
{
	return nullptr;
}

FGuid UMPlayer::GetConnId() const
{
	return FGuid();
}

void UMPlayer::SendToMe(const FGameMessage& InMessage) const
{
	if (!Session)
		return;
		
	Session->Send(InMessage);
}

bool UMPlayer::IsOnline() const
{
	return false;
}

bool UMPlayer::Load()
{
	return false;
}

void UMPlayer::Save()
{
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

void UMPlayer::OnOnline()
{
}

void UMPlayer::OnOffline()
{
}
