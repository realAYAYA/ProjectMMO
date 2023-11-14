#include "MWorld.h"

#include "MGameSession.h"

UMWorld::~UMWorld()
{
}

bool UMWorld::Init()
{
	return true;
}

void UMWorld::Cleanup()
{
}

void UMWorld::Online(UMGameSession* InSession)
{
	Session = InSession;
	Session->World = this;
}

void UMWorld::Offline(UMGameSession* InSession)
{
	check(Session == InSession);
	
	Save();
	
	Session = nullptr;
}

bool UMWorld::IsOnline() const
{
	return Session != nullptr;
}

UMGameSession* UMWorld::GetSession() const
{
	return Session;
}

FGuid UMWorld::GetConnId() const
{
	if (Session)
		return Session->ID;
	else
	{
		return FGuid();
	}
}

bool UMWorld::Load()
{
	return true;
}

bool UMWorld::Save()
{
	return false;
}

void UMWorld::Reset()
{
}

void UMWorld::OnOnline()
{
}

void UMWorld::OnOffline()
{
}
