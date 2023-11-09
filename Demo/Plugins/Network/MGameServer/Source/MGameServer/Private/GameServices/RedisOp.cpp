#include "RedisOp.h"
#include "MGameServerPrivate.h"

#include "Misc/Fnv.h"

#define CHECK_REDIS_CLIENT \
if (!GGameServerModule->RedisClient) \
{ \
	UE_LOG(LogMGameServer, Error, TEXT("[RedisOp] %s 错误,还未初始化RedisClient"), ANSI_TO_TCHAR(__FUNCTION__)); \
	return false; \
}

FString MakeKey(const FString& InKey)
{
#if WITH_EDITOR
	static FString MachineId;
	if (MachineId.IsEmpty())
	{
		const FString Name = FPlatformProcess::ComputerName();
		const uint64 HashId = FFnv::MemFnv64(*Name, Name.Len() * sizeof(TCHAR));
		MachineId = BytesToHex(reinterpret_cast<const uint8*>(&HashId), sizeof(HashId));
	}
	return FString::Printf(TEXT("PROJECT_M:%s_%s"), *MachineId, *InKey);  // 编辑器模式将当前机器名编码到KEY里面
	#else
	return FString::Printf(TEXT("IDLEZ_%s"), *InKey);
#endif	
}

FString GenerateAccountKey(const FString& InAccount)
{
	return MakeKey(FString::Printf(TEXT("ACC1_%s"), *InAccount));
}

bool FRedisOp::LoadPlayerData(uint64 RoleId, FMPlayerData* OutData)
{
	CHECK_REDIS_CLIENT

	
}

bool FRedisOp::SavePlayerData(uint64 RoleId, const FMPlayerData& InData)
{
	
}

bool FRedisOp::GetAccountInfo(const FString& InAccount, uint64* OutPlayerId)
{
	CHECK_REDIS_CLIENT
	
	TSet<FString> Fields;
	Fields.Emplace(TEXT("PlayerId"));

	TMap<FString, FString> ResultMap;
	
	const FString Key = GenerateAccountKey(InAccount);
	if (!GGameServerModule->RedisClient->HMGet(Key, Fields, ResultMap))
	{
		UE_LOG(LogMGameServer, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s"), *FString(__FUNCTION__), *Key);
		return false;
	}

	const auto PlayerId = ResultMap.Find(TEXT("PlayerId"));
	if (PlayerId && !PlayerId->IsEmpty())
		LexFromString(*OutPlayerId, GetData(*PlayerId));
	
	UE_LOG(LogMGameServer, Display, TEXT("[RedisOp] %s Account = %s UserId = %llu"), *FString(__FUNCTION__), *InAccount, *PlayerId);
	
	return true;
}

bool FRedisOp::SetAccountInfo(const FString& InAccount, const uint64 OutPlayerId)
{
	
}

bool FRedisOp::OccupyName(const FString& Name, const uint64 InId)
{
	
}

bool FRedisOp::GetOccupyNameId(const FString& Name, uint64& OutId)
{
	
}
