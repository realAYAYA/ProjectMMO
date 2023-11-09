#pragma once

#include "MGameTypes.h"

struct FRedisOp
{
	static bool LoadPlayerData(uint64 RoleId, FMPlayerData* OutData);
	static bool SavePlayerData(uint64 RoleId, const FMPlayerData& InData);

	static bool GetAccountInfo(const FString& InAccount, uint64* OutPlayerId);
	static bool SetAccountInfo(const FString& InAccount, const uint64 OutPlayerId);
	
	//static bool GeneratePlayerId(uint64* OutId);
	//static bool GenerateWorldSerialNum(uint64* OutId);

	static bool OccupyName(const FString& Name, const uint64 InId);
	static bool GetOccupyNameId(const FString& Name, uint64& OutId);
};
