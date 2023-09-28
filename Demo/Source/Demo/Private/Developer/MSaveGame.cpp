#include "MSaveGame.h"


bool UMSaveGame::CreateUser(const FString& InID, const FString& InName)
{
	if (INDEX_NONE != FindUserIndex(InID))
		return false;

	FMUserData Data;
	Data.UserID = InID;
	Data.UserName = InName;
	Data.CreateDate = FDateTime::Now().GetTicks();

	UserData.Add(Data);

	return true;
}

bool UMSaveGame::CreateUser(const FMUserData& InData)
{
	if (INDEX_NONE != FindUserIndex(InData.UserID))
		return false;

	UserData.Add(InData);
	return true;
}

void UMSaveGame::RemoveUser(const FString& InID)
{
	const int32 Index = FindUserIndex(InID);
	if (INDEX_NONE != Index)
		UserData.RemoveAt(Index);
}

FMUserData UMSaveGame::FindUserData(const FString& InID)
{
	const int32 Index = FindUserIndex(InID);
	if (INDEX_NONE == Index)
		return FMUserData();

	return UserData[Index];
}

FMUserData* UMSaveGame::FindUserDataRef(const FString& InID)
{
	const int32 Index = FindUserIndex(InID);
	if (INDEX_NONE == Index)
		return nullptr;

	return &UserData[Index];
}

FRoleData UMSaveGame::GetRoleData(const FString& InID, const FString& InName)
{
	const int32 Index = FindUserIndex(InID);
	if (INDEX_NONE == Index)
		return FRoleData();

	const int32 Index1 = FindRoleIndex(Index, InName);
	if (INDEX_NONE == Index1)
		return FRoleData();

	return UserData[Index].RoleData[Index1];
}

bool UMSaveGame::UpdateRoleName(const FString& InID, const FString& OldName, const FString& NewName)
{
	const int32 Index = FindUserIndex(InID);
	if (INDEX_NONE == Index)
		return false;

	const int32 Index1 = FindRoleIndex(Index, OldName);
	if (INDEX_NONE == Index1)
		return false;

	UserData[Index].RoleData[Index1].RoleName = NewName;
	return true;
}

bool UMSaveGame::CreateRole(const FString& InID, const FCreateRoleParams& InParams)
{
	const int32 Index = FindUserIndex(InID);
	if (INDEX_NONE == Index)
		return false;

	const int32 Index1 = FindRoleIndex(Index, InParams.RoleName);
	if (INDEX_NONE != Index1)
		return false;

	FRoleData NewRole;
	NewRole.RoleName = InParams.RoleName;
	NewRole.Class = InParams.Class;
	NewRole.Camp = ECamp::None;
	NewRole.Level = 1;
	NewRole.CreateDate = FDateTime::Now().GetTicks();

	UserData[Index].RoleData.Add(NewRole);
	return true;
}

bool UMSaveGame::UpdateRole(const FString& InID, const FString& InName, const FRoleData& InData)
{
	const int32 Index = FindUserIndex(InID);
	if (INDEX_NONE == Index)
		return false;

	const int32 Index1 = FindRoleIndex(Index, InName);
	if (INDEX_NONE == Index1)
		return false;

	UserData[Index].RoleData[Index1] = InData;
	return true;
}

void UMSaveGame::RemoveRole(const FString& InID, const FString& InName)
{
	const int32 Index = FindUserIndex(InID);
	if (INDEX_NONE == Index)
		return;

	const int32 Index1 = FindRoleIndex(Index, InName);
	if (INDEX_NONE == Index1)
		return;

	UserData[Index].RoleData.RemoveAt(Index1);
}

int32 UMSaveGame::FindUserIndex(const FString& InID)
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

int32 UMSaveGame::FindRoleIndex(const FString& InID, const FString& InName)
{
	const int32 Index = FindUserIndex(InID);
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

int32 UMSaveGame::FindRoleIndex(const int32 Index, const FString& InName)
{
	for (int32 i = 0; i < UserData[Index].RoleData.Num(); i++)
	{
		const FRoleData& Data = UserData[Index].RoleData[i];
		if (Data.RoleName == InName)
			return i;
	}

	return INDEX_NONE;
}
