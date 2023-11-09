#include "MWorld.h"

#include "MGameSession.h"

UMWorld::~UMWorld()
{
}

bool UMWorld::Init(int64 InUserId, const FString& InAccount, int64 InRoleId)
{
	return false;
}

void UMWorld::Cleanup()
{
}

void UMWorld::Online(UMGameSession* InSession)
{
}

void UMWorld::Offline(UMGameSession* InSession)
{
}

bool UMWorld::IsOnline() const
{
	return true;
}

UMGameSession* UMWorld::GetSession() const
{
	return Session;
}

FGuid UMWorld::GetConnId() const
{
	return Session->ID;
}

bool UMWorld::Load()
{
	return true;
}

void UMWorld::Save()
{
}

void UMWorld::OnOnline()
{
}

void UMWorld::OnOffline()
{
}
