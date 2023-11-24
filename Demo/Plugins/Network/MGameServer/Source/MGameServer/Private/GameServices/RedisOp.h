#pragma once

#include "MGameTypes.h"

struct FRedisOp
{
	static bool LoadPlayerData(const uint64 PlayerId, const FMPlayerData* OutData);
	static bool SavePlayerData(const uint64 PlayerId, const FMPlayerData& InData);

	static bool GetAccountInfo(const FString& InAccount, uint64* OutPlayerId);
	static bool SetAccountInfo(const FString& InAccount, const uint64 InPlayerId);
	
	//static bool GeneratePlayerId(uint64* OutId);
	//static bool GenerateWorldSerialNum(uint64* OutId);

	static bool OccupyName(const FString& Name, const int64 InId);
	static bool GetOccupyNameID(const FString& Name, int64& OutId);
};
