#include "RedisProxy.h"

#include "RedisClient.h"
/* 学习用代码，使用范例
#include "ZRedisOp.h"

#include <random>

#include "RedisClient.h"
#include "ZGameTablesModule.h"
#include "ZSoloArenaGlobal.h"
#include "ZSwordPkManager.h"
#include "ZTools.h"
#include "Misc/Fnv.h"

#define CHECK_REDISCLIENT \
		if (!GGameServiceModule->RedisClient) \
    	{ \
    		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,还未初始化RedisClient"), ANSI_TO_TCHAR(__FUNCTION__)); \
    		return false; \
    	}
    		

static constexpr TCHAR kRoleSaveDataField[] = TEXT("RoleSaveData");
static constexpr TCHAR kRoleBattleHistoryField[] = TEXT("RoleBattleHistory2");

FString MakeKey(const FString& InKey)
{
#if WITH_EDITOR
	static FString MachineId;
	if (MachineId.IsEmpty())
	{
		FString Name = FPlatformProcess::ComputerName();
		uint64 HashId = FFnv::MemFnv64(*Name, Name.Len() * sizeof(TCHAR));
		MachineId = BytesToHex(reinterpret_cast<const uint8*>(&HashId), sizeof(HashId));
	}
	return FString::Printf(TEXT("IDLEZ:%s_%s"), *MachineId, *InKey);  // 编辑器模式将当前机器名编码到KEY里面
#else
	return FString::Printf(TEXT("IDLEZ_%s"), *InKey);
#endif	
}

FString GenerateRoleKey(uint64 RoleId)
{
	return MakeKey(FString::Printf(TEXT("ROLE_%llu"), RoleId));
}

FString GenerateSeptKey(uint64 SeptId)
{
	return MakeKey(FString::Printf(TEXT("SEPT_%llu"), SeptId));
}

FString GenerateAccountKey(const FString& InAccount)
{
	return MakeKey(FString::Printf(TEXT("ACC1_%s"), *InAccount));
}

FString GenerateSoloHistoryKey(uint64 InHistoryWorldId)
{
	return MakeKey(FString::Printf(TEXT("SOLO3_%llu"), InHistoryWorldId));
}

FString GenerateShareItemKey(uint64 InShareId)
{
	return MakeKey(FString::Printf(TEXT("SHAREITEM_%llu"), InShareId));
}

FString GenerateSwordPkGlobalData()
{
	return MakeKey(TEXT("SWPKGLOBAL2"));
}

FString GenerateSeptGlobalData()
{
	return MakeKey(TEXT("SEPTGLOBAL"));
}

// ============================================================================

bool FZRedisOp::LoadRoleData(uint64 RoleId, idlezt::RoleSaveData* OutRoleSaveData)
{
	CHECK_REDISCLIENT;
	
	TArray<char> OutValue;
	
	const FString Key = GenerateRoleKey(RoleId);
	if (!GGameServiceModule->RedisClient->HGetBin(Key, kRoleSaveDataField, OutValue))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s"), ANSI_TO_TCHAR(__FUNCTION__), *Key);	
		return false;
	}

	if (!OutRoleSaveData->ParseFromArray(OutValue.GetData(), OutValue.Num()))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,反序列化失败 Key=%s Size=%d"), ANSI_TO_TCHAR(__FUNCTION__), *Key, OutValue.Num());	
		return false;
	}
	
	return true;
}

bool FZRedisOp::SaveRoleData(uint64 RoleId, const idlezt::RoleSaveData& InRoleSaveData)
{
	CHECK_REDISCLIENT;

	int32 Size = InRoleSaveData.ByteSizeLong();
	TArray<char> Buffer;
	Buffer.SetNumUninitialized(Size);
	
	if (!InRoleSaveData.SerializeToArray(Buffer.GetData(), Size))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,序列化失败 RoldId=%llu"), ANSI_TO_TCHAR(__FUNCTION__), RoleId);
		return false;
	}
	
	const FString Key = GenerateRoleKey(RoleId);
	if (!GGameServiceModule->RedisClient->HSetBin(Key, kRoleSaveDataField, Buffer))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s Size=%d"), ANSI_TO_TCHAR(__FUNCTION__), *Key, Size);
		return false;
	}
		
	return true;	
}

bool FZRedisOp::GetAccountInfo(const FString& InAccount, uint64* OutUserId, uint64* OutRoleId)
{
	CHECK_REDISCLIENT;
	
	TSet<FString> Fields;
	Fields.Emplace(TEXT("UserId"));
	Fields.Emplace(TEXT("RoleId"));

	TMap<FString, FString> ResultMap;
	
	const FString Key = GenerateAccountKey(InAccount);
	if (!GGameServiceModule->RedisClient->HMGet(Key, Fields, ResultMap))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s"), ANSI_TO_TCHAR(__FUNCTION__), *Key);
		return false;
	}

	auto UserId = ResultMap.Find(TEXT("UserId"));
	auto RoleId = ResultMap.Find(TEXT("RoleId"));
	if (UserId && !UserId->IsEmpty())
		LexFromString(*OutUserId, GetData(*UserId));
	if (RoleId && !RoleId->IsEmpty())
		LexFromString(*OutRoleId, GetData(*RoleId));
	
	UE_LOG(LogZGameService, Display, TEXT("[RedisOp] %s Account=%s UserId=%llu RoleId=%llu"), ANSI_TO_TCHAR(__FUNCTION__), *InAccount, *OutUserId, *OutRoleId);
	return true;
}

bool FZRedisOp::SetAccountInfo(const FString& InAccount, uint64 InUserId, uint64 InRoleId)
{
	CHECK_REDISCLIENT;
	
	TMap<FString, FString> MemberMap;
	MemberMap.Emplace(TEXT("UserId"), LexToString(InUserId));
	MemberMap.Emplace(TEXT("RoleId"), LexToString(InRoleId));

	const FString Key = GenerateAccountKey(InAccount);
	if (!GGameServiceModule->RedisClient->HMSet(Key, MemberMap))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s"), ANSI_TO_TCHAR(__FUNCTION__), *Key);
		return false;
	}

	UE_LOG(LogZGameService, Display, TEXT("[RedisOp] %s Account=%s UserId=%llu RoleId=%llu"), ANSI_TO_TCHAR(__FUNCTION__), *InAccount, InUserId, InRoleId);
	return true;
}

bool FZRedisOp::GenerateRoleId(uint64* OutId)
{
    CHECK_REDISCLIENT;
	
   	int64 Out = 0;
	const FString Key = MakeKey(TEXT("IdGenerator"));
   	if (!GGameServiceModule->RedisClient->HIncrby(Key, TEXT("Role"), 1, Out))
   	{
   		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败"), ANSI_TO_TCHAR(__FUNCTION__));
   		return false;
   	}
    
   	if (Out == 0)
   		return false;

	*OutId = ZGenerateEntityId(idlezt::ET_Player, Out);
	
   	return true;
}

bool FZRedisOp::GenerateWorldSerialNum(uint64* OutId)
{
	CHECK_REDISCLIENT;
	
	int64 Out = 0;
	const FString Key = MakeKey(TEXT("IdGenerator"));
	if (!GGameServiceModule->RedisClient->HIncrby(Key, TEXT("World"), 1, Out))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}
    
	if (Out == 0)
		return false;

	*OutId = Out;
	
	return true;
}

bool FZRedisOp::GenerateSeptId(uint64* OutId)
{
	CHECK_REDISCLIENT;
	
	int64 Out = 0;
	const FString Key = MakeKey(TEXT("IdGenerator"));
	if (!GGameServiceModule->RedisClient->HIncrby(Key, TEXT("Sept"), 1, Out))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}
    
	if (Out == 0)
		return false;

	*OutId = Out;
	
	return true;	
}

bool FZRedisOp::OccupyName(const FString& Name, int64 InId)
{
	CHECK_REDISCLIENT;

	std::string AsciiName;
	ZToString(Name, &AsciiName);
	uint64 NameHash = FFnv::MemFnv64(AsciiName.c_str(), AsciiName.size());
	
	const FString Key = MakeKey(FString::Printf(TEXT("UQNAME_%llu"), NameHash));
	if (!GGameServiceModule->RedisClient->SetNxInt(Key, InId))
	{
		return false;
	}
	
	return true;	
}

bool FZRedisOp::GetOccupyNameId(const FString& Name, int64& OutId)
{
	CHECK_REDISCLIENT;

	std::string AsciiName;
	ZToString(Name, &AsciiName);
	uint64 NameHash = FFnv::MemFnv64(AsciiName.c_str(), AsciiName.size());

	const FString Key = MakeKey(FString::Printf(TEXT("UQNAME_%llu"), NameHash));
	if (!GGameServiceModule->RedisClient->GetInt(Key, OutId))
	{
		return false;
	}

	return true;
}

bool FZRedisOp::LoadLeaderboardData(FZLeaderboardSaveData& OutData, const FString& DataField)
{
	CHECK_REDISCLIENT
	
	TArray<char> OutValue;

	const FString Key = MakeKey(TEXT("LoadLeaderboard"));
	if (!GGameServiceModule->RedisClient->HGetBin(Key, DataField, OutValue))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s"), ANSI_TO_TCHAR(__FUNCTION__), *Key);
		return false;
	}

	idlezt::LeaderboardSaveData OutLeaderboardSaveData;
	
	if (!OutLeaderboardSaveData.ParseFromArray(OutValue.GetData(), OutValue.Num()))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,反序列化失败 Key=%s Size=%d"), ANSI_TO_TCHAR(__FUNCTION__), *Key, OutValue.Num());	
		return false;
	}

	OutData.FromPb(OutLeaderboardSaveData);
	
	return true;
}

bool FZRedisOp::SaveLeaderboardData(const FZLeaderboardSaveData& InData, const FString& DataField)
{
	CHECK_REDISCLIENT;

	idlezt::LeaderboardSaveData SaveData;
	InData.ToPb(&SaveData);
	const int32 Size = SaveData.ByteSizeLong();
	TArray<char> Buffer;
	Buffer.SetNumUninitialized(Size);
	
	if (!SaveData.SerializeToArray(Buffer.GetData(), Size))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,序列化失败,排行榜"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}

	const FString Key = MakeKey(TEXT("LoadLeaderboard"));
	if (!GGameServiceModule->RedisClient->HSetBin(Key, DataField, Buffer))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s Size=%d"), ANSI_TO_TCHAR(__FUNCTION__), *Key, Size);
		return false;
	}
		
	return true;	
}

bool FZRedisOp::LoadChatData(FZChatData& OutData, FZRolePrivateChatRecordSaveData& OutData2)
{
	CHECK_REDISCLIENT
	
	TArray<char> OutValue, OutValue2;

	const FString& DataField("ChatData");
	const FString Key = MakeKey(TEXT("Public"));
	const FString Key2 = MakeKey(TEXT("Private"));
	if (!GGameServiceModule->RedisClient->HGetBin(Key, DataField, OutValue))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s,公共聊天记录"), ANSI_TO_TCHAR(__FUNCTION__), *Key);
		return false;
	}
	if (!GGameServiceModule->RedisClient->HGetBin(Key2, DataField, OutValue2))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s,私人聊天记录"), ANSI_TO_TCHAR(__FUNCTION__), *Key2);
		return false;
	}

	idlezt::ChatData OutChatSaveData;
	idlezt::RolePrivateChatRecordSaveData OutPrivateChatSaveData;
	if (!OutChatSaveData.ParseFromArray(OutValue.GetData(), OutValue.Num()))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,反序列化失败 Key=%s Size=%d,公共聊天记录"), ANSI_TO_TCHAR(__FUNCTION__), *Key, OutValue.Num());	
		return false;
	}
	if (!OutPrivateChatSaveData.ParseFromArray(OutValue2.GetData(), OutValue2.Num()))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,反序列化失败 Key=%s Size=%d,私人聊天记录"), ANSI_TO_TCHAR(__FUNCTION__), *Key2, OutValue2.Num());	
		return false;
	}

	OutData.FromPb(OutChatSaveData);
	OutData2.FromPb(OutPrivateChatSaveData);
	
	return true;
}

bool FZRedisOp::SaveChatData(const FZChatData& InData, const FZRolePrivateChatRecordSaveData& InData2)
{
	CHECK_REDISCLIENT;

	idlezt::ChatData SaveData;
	idlezt::RolePrivateChatRecordSaveData SaveData2;

	TArray<char> Buffer, Buffer2;
	InData.ToPb(&SaveData);
	InData2.ToPb(&SaveData2);
	const int32 Size = SaveData.ByteSizeLong();
	Buffer.SetNumUninitialized(Size);
	const int32 Size2 = SaveData2.ByteSizeLong();
	Buffer2.SetNumUninitialized(Size2);
	
	if (!SaveData.SerializeToArray(Buffer.GetData(), Size))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,序列化失败,公共聊天记录"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}

	if (!SaveData2.SerializeToArray(Buffer2.GetData(), Size2))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,序列化失败,私人聊天记录"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}

	const FString& DataField("ChatData");
	const FString Key = MakeKey(TEXT("Public"));
	const FString Key2 = MakeKey(TEXT("Private"));
	if (!GGameServiceModule->RedisClient->HSetBin(Key, DataField, Buffer))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s Size=%d,公共聊天记录"), ANSI_TO_TCHAR(__FUNCTION__), *Key, Size);
		return false;
	}
	if (!GGameServiceModule->RedisClient->HSetBin(Key2, DataField, Buffer2))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s Size=%d,私人聊天记录"), ANSI_TO_TCHAR(__FUNCTION__), *Key2, Size2);
		return false;
	}
	
	return true;
}

bool FZRedisOp::RecordBattleHistory(const idlezt::BattleHistory& InHistory)
{
	CHECK_REDISCLIENT;

	const uint64 WorldId = InHistory.info().world_id();
	const FString Key = GenerateSoloHistoryKey(WorldId);

	int32 DataSize = InHistory.ByteSizeLong();
	TArray<char> DataBuffer;
	DataBuffer.SetNumUninitialized(DataSize);
	if (!InHistory.SerializeToArray(DataBuffer.GetData(), DataSize))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,序列化失败 Key=%s"), ANSI_TO_TCHAR(__FUNCTION__), *Key);
		return false;
	}
	
	int32 CompressedSize = FCompression::CompressMemoryBound(NAME_Zlib, DataBuffer.Num());
	
	TArray<char> CompressedBuffer;
	CompressedBuffer.SetNumUninitialized(CompressedSize);
	
	if (FCompression::CompressMemory(NAME_Zlib, CompressedBuffer.GetData(), CompressedSize, DataBuffer.GetData(), DataBuffer.Num()))
	{
		CompressedBuffer.SetNum(CompressedSize, false);
	}
	else
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,数据压缩失败 Key=%s"), ANSI_TO_TCHAR(__FUNCTION__), *Key);
		return false;
	}

	ZINFO(TEXT("[RedisOp] RecordBattleHistory WorldId={} DataSize={} CompressedSize={}"), WorldId, DataSize, CompressedSize);

	idlezt::CompressedData SaveData;
	SaveData.set_original_size(DataBuffer.Num());
	SaveData.set_compressed_data(CompressedBuffer.GetData(), CompressedBuffer.Num());

	int32 SaveDataSize = SaveData.ByteSizeLong();
	TArray<char> SaveDataBuffer;
	SaveDataBuffer.SetNumUninitialized(SaveDataSize);
	SaveData.SerializeToArray(SaveDataBuffer.GetData(), SaveDataSize);

	const int32 KeepTime = FZGameTablesModule::Get().GetGameTables()->SoloArenaGlobal->FriendlySolo->RecordKeepTime.GetTotalSeconds() + 60;
	if (!GGameServiceModule->RedisClient->SetBin(Key, SaveDataBuffer, KeepTime))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s Size=%d"), ANSI_TO_TCHAR(__FUNCTION__), *Key, DataSize);
		return false;
	}
	
	return true;
}

bool FZRedisOp::FetchBattleHistory(uint64 HistoryWorldId, idlezt::CompressedData* OutData)
{
	CHECK_REDISCLIENT;

	const FString Key = GenerateSoloHistoryKey(HistoryWorldId);

	TArray<char> OutValue;
	if (!GGameServiceModule->RedisClient->GetBin(Key, OutValue))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s"), ANSI_TO_TCHAR(__FUNCTION__), *Key);	
		return false;
	}
	
	if (!OutData->ParseFromArray(OutValue.GetData(), OutValue.Num()))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,反序列化失败 Key=%s Size=%d"), ANSI_TO_TCHAR(__FUNCTION__), *Key, OutValue.Num());	
		return false;
	}

	ZINFO(TEXT("[RedisOp] FetchBattleHistory WorldId={} DataSize={} CompressedSize={}"),
		HistoryWorldId, OutData->original_size(), OutData->compressed_data().size());
	
	return true;
}

FString GenerateRoleBattleHistoryField(EZSoloType SoloType)
{
	return FString::Printf(TEXT("%s_%d"), kRoleBattleHistoryField, static_cast<int32>(SoloType));
}

bool LoadRoleBattleHistory(EZSoloType SoloType, uint64 RoleId, idlezt::RoleBattleHistorySaveData* OutSaveData)
{
	CHECK_REDISCLIENT;
	TArray<char> OutValue;
	const FString Key = GenerateRoleKey(RoleId);
	const FString Field = GenerateRoleBattleHistoryField(SoloType);
	if (!GGameServiceModule->RedisClient->HGetBin(Key, Field, OutValue))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s"), ANSI_TO_TCHAR(__FUNCTION__), *Key);	
		return false;
	}

	idlezt::CompressedData SaveData;
	if (!SaveData.ParseFromArray(OutValue.GetData(), OutValue.Num()))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,反序列化失败1 Key=%s Size=%d"), ANSI_TO_TCHAR(__FUNCTION__), *Key, OutValue.Num());
		return false;
	}

	TArray<char> UncompressedBuffer;  // 存放解压的数据
	UncompressedBuffer.SetNumUninitialized(SaveData.original_size());
	if (!FCompression::UncompressMemory(NAME_Zlib, UncompressedBuffer.GetData(), UncompressedBuffer.Num(), SaveData.compressed_data().c_str(), SaveData.compressed_data().length()))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,解压失败 Key=%s Size=%d"), ANSI_TO_TCHAR(__FUNCTION__), *Key, OutValue.Num());
	}
	
	if (!OutSaveData->ParseFromArray(UncompressedBuffer.GetData(), UncompressedBuffer.Num()))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,反序列化失败2 Key=%s Size=%d"), ANSI_TO_TCHAR(__FUNCTION__), *Key, OutValue.Num());	
		return false;
	}
	
	return true;	
}

bool SaveRoleBattleHistory(EZSoloType SoloType, uint64 RoleId, const idlezt::RoleBattleHistorySaveData& InSaveData)
{
	CHECK_REDISCLIENT;
	int32 DataSize = InSaveData.ByteSizeLong();
	TArray<char> DataBuffer;
	DataBuffer.SetNumUninitialized(DataSize);
	if (!InSaveData.SerializeToArray(DataBuffer.GetData(), DataSize))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,序列化失败 RoldId=%llu"), ANSI_TO_TCHAR(__FUNCTION__), RoleId);
		return false;
	}

	int32 CompressedSize = FCompression::CompressMemoryBound(NAME_Zlib, DataBuffer.Num());
	
	TArray<char> CompressedBuffer;
	CompressedBuffer.SetNumUninitialized(CompressedSize);
	
	if (FCompression::CompressMemory(NAME_Zlib, CompressedBuffer.GetData(), CompressedSize, DataBuffer.GetData(), DataBuffer.Num()))
	{
		CompressedBuffer.SetNum(CompressedSize, false);
	}
	else
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,数据压缩失败 RoleId=%llu"), ANSI_TO_TCHAR(__FUNCTION__), RoleId);
		return false;
	}
	
	idlezt::CompressedData SaveData;
	SaveData.set_original_size(DataBuffer.Num());
	SaveData.set_compressed_data(CompressedBuffer.GetData(), CompressedBuffer.Num());

	int32 SaveDataSize = SaveData.ByteSizeLong();
	TArray<char> SaveDataBuffer;
	SaveDataBuffer.SetNumUninitialized(SaveDataSize);
	SaveData.SerializeToArray(SaveDataBuffer.GetData(), SaveDataSize);
	
	const FString Key = GenerateRoleKey(RoleId);
	const FString Field = GenerateRoleBattleHistoryField(SoloType);
	if (!GGameServiceModule->RedisClient->HSetBin(Key, Field, SaveDataBuffer))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s Size=%d"), ANSI_TO_TCHAR(__FUNCTION__), *Key, DataSize);
		return false;
	}
	
	return true;
}

bool TryCleanupBattleHistorySaveData(EZSoloType SoloType, uint64 RoleId, idlezt::RoleBattleHistorySaveData* SaveData)
{
	bool bChanged = false;  // 是否发生了清理行为
	
	int32 MaxNum = FZGameTablesModule::Get().GetGameTables()->SoloArenaGlobal->FriendlySolo->RecordKeepNum;
	while (SaveData->histories_size() > MaxNum)
	{
		bChanged = true;
		
		auto Iter = SaveData->histories().begin();
		ZINFO(TEXT("[RedisOp] TryCleanupBattleHistorySaveData 清理多余的战斗记录 RoleId={} WorldId={} Size={}"), RoleId, Iter->base().world_id(), SaveData->histories().size());
		SaveData->mutable_histories()->erase(Iter);
	}

	int32 RoundNum = 0;
	if (SoloType == EZSoloType::SoloType_SwordPk)
		RoundNum = UZSwordPkManager::Get()->GetRoundNum();
	
	auto Now = ZNow();
	auto KeepTime = FZGameTablesModule::Get().GetGameTables()->SoloArenaGlobal->FriendlySolo->RecordKeepTime;
	while (SaveData->histories_size() > 0)
	{
		auto Iter = SaveData->histories().begin();
		
		FDateTime ExpireTime(Iter->base().begin_ticks());
		ExpireTime += KeepTime;
		if (Now < (ExpireTime))  // 检查是否过期
		{
			if (RoundNum == 0 || RoundNum == Iter->round_num())  // 检查赛季是否匹配
			{
				break;  // 按顺序添加的，第一个不满足删除条件后面的也肯定不满足
			}
		}
		bChanged = true;
		
		ZINFO(TEXT("[RedisOp] TryCleanupBattleHistorySaveData 清理过期战斗记录 RoleId={} WorldId={} ExpireTime={} Round={},{}"), RoleId, Iter->base().world_id(), ExpireTime.ToString(), Iter->round_num(), RoundNum);
		SaveData->mutable_histories()->erase(Iter);
	}

	return bChanged;
}

bool FZRedisOp::RoleAddBattleHistory(EZSoloType SoloType, uint64 RoleId, const idlezt::BattleInfo& InInfo, bool bCanRevenge, int32 RoundNum)
{
	CHECK_REDISCLIENT;
	
	idlezt::RoleBattleHistorySaveData SaveData;
	LoadRoleBattleHistory(SoloType, RoleId, &SaveData);
	
	auto* SaveInfo = SaveData.add_histories();
	SaveInfo->mutable_base()->CopyFrom(InInfo);
	SaveInfo->set_can_revenge(bCanRevenge);
	SaveInfo->set_round_num(RoundNum);

	TryCleanupBattleHistorySaveData(SoloType, RoleId, &SaveData);

	return SaveRoleBattleHistory(SoloType, RoleId, SaveData);
}

bool FZRedisOp::RoleGetBattleHistoryList(EZSoloType SoloType, uint64 RoleId, idlezt::RoleBattleHistoryList* OutData)
{
	CHECK_REDISCLIENT;
	
	idlezt::RoleBattleHistorySaveData SaveData;
	LoadRoleBattleHistory(SoloType, RoleId, &SaveData);

	bool bNeedSave = TryCleanupBattleHistorySaveData(SoloType, RoleId, &SaveData);
	
	for (const auto& Info : SaveData.histories())
	{
		OutData->add_entries()->CopyFrom(Info);
	}

	if (bNeedSave)
	{
		SaveRoleBattleHistory(SoloType, RoleId, SaveData);
	}
	
	return true;
}

bool FZRedisOp::RoleResetBattleHistoryList(EZSoloType SoloType, uint64 RoleId)
{
	CHECK_REDISCLIENT;
	
	idlezt::RoleBattleHistorySaveData SaveData;
	return SaveRoleBattleHistory(SoloType, RoleId, SaveData);	
}

bool FZRedisOp::GetRoleBattleInfo(EZSoloType SoloType, uint64 RoleId, uint64 WorldId, idlezt::RoleBattleInfo* OutData)
{
	idlezt::RoleBattleHistorySaveData SaveData;
	if (!LoadRoleBattleHistory(SoloType, RoleId, &SaveData))
		return false;
	for (const auto& Info : SaveData.histories())
	{
		if (Info.base().world_id() == WorldId)
		{
			OutData->CopyFrom(Info);
			return true;
		}
	}
	return false;
}

bool FZRedisOp::UpdateRoleBattleInfo(EZSoloType SoloType, uint64 RoleId, const idlezt::RoleBattleInfo& InData)
{
	idlezt::RoleBattleHistorySaveData SaveData;
	if (!LoadRoleBattleHistory(SoloType, RoleId, &SaveData))
		return false;
	bool bOk = false;
	{
		auto* Histories = SaveData.mutable_histories();
		for (auto Iter = Histories->begin(); Iter != Histories->end(); ++Iter) 
		{
			if (Iter->base().world_id() == InData.base().world_id())
			{
				Iter->CopyFrom(InData);
				bOk = true;
				break;
			}
		}
	}
	if (!bOk)
	{
		return false;
	}
	return SaveRoleBattleHistory(SoloType, RoleId, SaveData);
}

bool FZRedisOp::AddShareItem(const idlezt::ItemData& InData, uint64* OutShareId)
{
	CHECK_REDISCLIENT;

	int32 DataSize = InData.ByteSizeLong();
	TArray<char> DataBuffer;
	DataBuffer.SetNumUninitialized(DataSize);
	if (!InData.SerializeToArray(DataBuffer.GetData(), DataSize))
	{
		ZERROR(TEXT("[RedisOp][道具分享] AddShareItem 序列化数据失败"));
		return false;
	}
	
	int64 ShareId = 0;
	{
		const FString Key = MakeKey(TEXT("IdGenerator"));
		if (!GGameServiceModule->RedisClient->HIncrby(Key, TEXT("ShareItem"), 1, ShareId))
		{
			ZERROR(TEXT("[RedisOp][道具分享] AddShareItem Redis操作失败"));
			return false;
		}
		if (ShareId == 0)
		{
			ZERROR(TEXT("[RedisOp][道具分享] AddShareItem ShareId生成失败"));
			return false;
		}
	}

	FString Key = GenerateShareItemKey(ShareId);
	int32 ExpireTime = FZGameTablesModule::Get().GetGameTables()->CommonConstants.ShareItemExpireTime;
	if (!GGameServiceModule->RedisClient->SetBin(Key, DataBuffer, ExpireTime))
	{
		ZERROR(TEXT("[RedisOp][道具分享] Redis操作失败 Key={} ExpireTime={}"), Key, ExpireTime);
		return false;
	}
	
	*OutShareId = ShareId;
	return true;
}

bool FZRedisOp::GetShareItem(uint64 InShareId, idlezt::ItemData* OutData)
{
	CHECK_REDISCLIENT;

	FString Key = GenerateShareItemKey(InShareId);
	TArray<char> DataBuffer;
	if (!GGameServiceModule->RedisClient->GetBin(Key, DataBuffer))
	{
		ZWARN(TEXT("[RedisOp][道具分享] GetShareItem 获取道具失败 Key={}"), Key);
		return false;
	}	

	if (!OutData->ParseFromArray(DataBuffer.GetData(), DataBuffer.Num()))
	{
		ZERROR(TEXT("[RedisOp][道具分享] GetShareItem 解析数据失败 Size={}"), DataBuffer.Num());
		return false;
	}
	
	return true;
}

bool FZRedisOp::LoadSwordPkGlobalData(idlezt::SwordPkGlobalSaveData* OutData)
{
	CHECK_REDISCLIENT;

	FString Key = GenerateSwordPkGlobalData();
	TArray<char> DataBuffer;
	if (!GGameServiceModule->RedisClient->GetCompressBin(Key, DataBuffer))
	{
		ZWARN(TEXT("[RedisOp][SwordPk] LoadSwordPkGlobalData 获取数据失败 Key={}"), Key);
		return false;
	}	

	if (!OutData->ParseFromArray(DataBuffer.GetData(), DataBuffer.Num()))
	{
		ZERROR(TEXT("[RedisOp][SwordPk] LoadSwordPkGlobalData 解析数据失败 Size={}"), DataBuffer.Num());
		return false;
	}
	
	return true;	
}

bool FZRedisOp::SaveSwordPkGlobalData(const idlezt::SwordPkGlobalSaveData& InData)
{
	CHECK_REDISCLIENT;

	int32 DataSize = InData.ByteSizeLong();
	TArray<char> DataBuffer;
	DataBuffer.SetNumUninitialized(DataSize);
	if (!InData.SerializeToArray(DataBuffer.GetData(), DataSize))
	{
		ZERROR(TEXT("[RedisOp][SwordPk] SaveSwordPkGlobalData 序列化数据失败"));
		return false;
	}
	
	FString Key = GenerateSwordPkGlobalData();
	if (!GGameServiceModule->RedisClient->SetCompressBin(Key, DataBuffer))
	{
		ZERROR(TEXT("[RedisOp][SwordPk] SaveSwordPkGlobalData Redis操作失败"));
		return false;
	}
	return true;
}

bool FZRedisOp::LoadSeptData(uint64 SeptId, idlezt::SeptSaveData* OutSeptSaveData)
{
	CHECK_REDISCLIENT;
	
	TArray<char> OutValue;
	
	const FString Key = GenerateSeptKey(SeptId);
	if (!GGameServiceModule->RedisClient->GetCompressBin(Key, OutValue))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s"), ANSI_TO_TCHAR(__FUNCTION__), *Key);	
		return false;
	}

	if (!OutSeptSaveData->ParseFromArray(OutValue.GetData(), OutValue.Num()))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,反序列化失败 Key=%s Size=%d"), ANSI_TO_TCHAR(__FUNCTION__), *Key, OutValue.Num());	
		return false;
	}
	
	return true;	
}

bool FZRedisOp::SaveSeptData(uint64 SeptId, const idlezt::SeptSaveData& InSeptSaveData)
{
	CHECK_REDISCLIENT;

	int32 Size = InSeptSaveData.ByteSizeLong();
	TArray<char> Buffer;
	Buffer.SetNumUninitialized(Size);
	
	if (!InSeptSaveData.SerializeToArray(Buffer.GetData(), Size))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,序列化失败 RoldId=%llu"), ANSI_TO_TCHAR(__FUNCTION__), SeptId);
		return false;
	}
	
	const FString Key = GenerateSeptKey(SeptId);
	if (!GGameServiceModule->RedisClient->SetCompressBin(Key, Buffer))
	{
		UE_LOG(LogZGameService, Error, TEXT("[RedisOp] %s 错误,Redis操作失败 Key=%s Size=%d"), ANSI_TO_TCHAR(__FUNCTION__), *Key, Size);
		return false;
	}
		
	return true;		
}

bool FZRedisOp::DeleteSeptData(uint64 SeptId)
{
	const FString Key = GenerateSeptKey(SeptId);
	GGameServiceModule->RedisClient->DelKey(Key);
	return true;
}

bool FZRedisOp::LoadSeptGlobalData(idlezt::SeptGlobalSaveData* OutData)
{
	CHECK_REDISCLIENT;

	FString Key = GenerateSeptGlobalData();
	TArray<char> DataBuffer;
	if (!GGameServiceModule->RedisClient->GetCompressBin(Key, DataBuffer))
	{
		ZWARN(TEXT("[RedisOp][Sept] LoadSeptGlobalData 获取数据失败 Key={}"), Key);
		return false;
	}	

	if (!OutData->ParseFromArray(DataBuffer.GetData(), DataBuffer.Num()))
	{
		ZERROR(TEXT("[RedisOp][Sept] LoadSeptGlobalData 解析数据失败 Size={}"), DataBuffer.Num());
		return false;
	}
	
	return true;	
}

bool FZRedisOp::SaveSeptGlobalData(const idlezt::SeptGlobalSaveData& InData)
{
	CHECK_REDISCLIENT;

	int32 DataSize = InData.ByteSizeLong();
	TArray<char> DataBuffer;
	DataBuffer.SetNumUninitialized(DataSize);
	if (!InData.SerializeToArray(DataBuffer.GetData(), DataSize))
	{
		ZERROR(TEXT("[RedisOp][Sept] SaveSeptGlobalData 序列化数据失败"));
		return false;
	}
	
	FString Key = GenerateSeptGlobalData();
	if (!GGameServiceModule->RedisClient->SetCompressBin(Key, DataBuffer))
	{
		ZERROR(TEXT("[RedisOp][Sept] SaveSeptGlobalData Redis操作失败"));
		return false;
	}
	return true;	
}

 */


URedisProxy::URedisProxy()
{
	RedisClient = MakeUnique<FRedisClient>();
	Port = 0;
	LastTime = FDateTime::Now();
}

bool URedisProxy::Connect(const FString& InIP, const FString& InPassword, const int32 InPort)
{
	IP = InIP;
	Password = InPassword;
	Port = InPort;
	RedisClient->ConnectToRedis(IP, Port, Password);
	return false;
}

bool URedisProxy::Disconnect()
{
	return true;
}

bool URedisProxy::ExecuteCmd(const FString& Cmd)
{
	return false;
}

/**
bool URedisProxy::Tick(float)
{
	LastTickTime = Now;
	
	if (Now > NextRedisAliveCheckTime)
	{
		NextRedisAliveCheckTime = Now + FTimespan::FromSeconds(5);
		if (RedisClient)
		{
			if (!RedisClient->ExecCommand("PING"))
			{
				FString RedisIp = FZGameTablesModule::Get().GetGameTables()->GameServiceConfig.RedisIp;
				int32 RedisPort = FZGameTablesModule::Get().GetGameTables()->GameServiceConfig.RedisPort;
				FString RedisPassword = FZGameTablesModule::Get().GetGameTables()->GameServiceConfig.RedisPassword;
				RedisClient->ConnectToRedis(RedisIp, RedisPort, RedisPassword);
			}
		}
	}
}
*/