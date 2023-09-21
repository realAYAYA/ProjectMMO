#include "MSaveGame.h"


FMUserData USaveGameMain::CreateUser(const FString& InUserID, const FString& InName)
{
	FMUserData* User = UserData.Find(InUserID);
}

FMUserData USaveGameMain::RemoveUser(const FString& InUserID)
{
}

FMUserData USaveGameMain::GetUserData(const FString& InUserID)
{
	for (const FMUserData& Data : UserData)
	{
		if (Data.UserID == InUserID)
			return Data;
	}

	return FMUserData();
}

FMCharacterData USaveGameMain::GetCharacter(const FString& InUserID, const FString& InName)
{
}

bool USaveGameMain::UpdateCharacterName(const FString& InUserID, const FString& OldName, const FString& NewName)
{
}

bool USaveGameMain::UpdateCharacter(const FString& InUserID, const FString& InName, const FMCharacterData& InData)
{
}

void USaveGameMain::RemoveCharacter(const FString& InUserID, const FString& InName)
{
}
