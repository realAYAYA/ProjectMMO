#include "MSaveGame.h"


bool UMSaveGame::CreateUser(const int64 InID, const FString& InName)
{
	if (INDEX_NONE != FindUser(InID))
		return false;

	return true;
}

void UMSaveGame::RemoveUser(const int64 InID)
{
	const int32 Index = FindUser(InID);
	if (INDEX_NONE != Index)
		UserData.RemoveAt(Index);
}

FMUserData UMSaveGame::GetUserData(const int64 InID)
{
	const int32 Index = FindUser(InID);
	if (INDEX_NONE == Index)
		return FMUserData();

	return UserData[Index];
}

FRoleData UMSaveGame::GetRoleData(const int64 InID, const FString& InName)
{
	const int32 Index = FindUser(InID);
	if (INDEX_NONE == Index)
		return FRoleData();

	const int32 Index1 = FindRole(Index, InName);
	if (INDEX_NONE == Index1)
		return FRoleData();

	return UserData[Index].RoleData[Index1];
}

bool UMSaveGame::UpdateRoleName(const int64 InID, const FString& OldName, const FString& NewName)
{
	const int32 Index = FindUser(InID);
	if (INDEX_NONE == Index)
		return false;

	const int32 Index1 = FindRole(Index, OldName);
	if (INDEX_NONE == Index1)
		return false;

	UserData[Index].RoleData[Index1].RoleName = NewName;
	return true;
}

bool UMSaveGame::UpdateRole(const int64 InID, const FString& InName, const FRoleData& InData)
{
	const int32 Index = FindUser(InID);
	if (INDEX_NONE == Index)
		return false;

	const int32 Index1 = FindRole(Index, InName);
	if (INDEX_NONE == Index1)
		return false;

	UserData[Index].RoleData[Index1] = InData;
	return true;
}

void UMSaveGame::RemoveRole(const int64 InID, const FString& InName)
{
	const int32 Index = FindUser(InID);
	if (INDEX_NONE == Index)
		return;

	const int32 Index1 = FindRole(Index, InName);
	if (INDEX_NONE == Index1)
		return;

	UserData[Index].RoleData.RemoveAt(Index1);
}

int32 UMSaveGame::FindUser(const int64 InID)
{
	for (int32 i = 0; i < UserData.Num(); i++)
	{
		const FMUserData& Data = UserData[i];
		if (Data.UserID == InID)
		{
			return i;
		}
	}

	return INDEX_NONE;
}

int32 UMSaveGame::FindUser(const FString& InName)
{
	for (int32 i = 0; i < UserData.Num(); i++)
	{
		const FMUserData& Data = UserData[i];
		if (Data.UserName == InName)
		{
			return i;
		}
	}

	return INDEX_NONE;
}

int32 UMSaveGame::FindRole(const int64 InID, const FString& InName)
{
	const int32 Index = FindUser(InID);
	if (INDEX_NONE == Index)
		return INDEX_NONE;

	for (int32 i = 0; i < UserData.Num(); i++)
	{
		const FRoleData& Data = UserData[Index].RoleData[i];
		if (Data.RoleName == InName)
			return i;
	}

	return INDEX_NONE;
}

int32 UMSaveGame::FindRole(const int32 Index, const FString& InName)
{
	for (int32 i = 0; i < UserData.Num(); i++)
	{
		const FRoleData& Data = UserData[Index].RoleData[i];
		if (Data.RoleName == InName)
			return i;
	}

	return INDEX_NONE;
}
