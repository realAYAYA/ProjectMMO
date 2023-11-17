#include "MPlayer.h"

#include "MGameSession.h"
#include "MPlayerManager.h"
#include "MTools.h"
#include "RedisOp.h"

UMPlayer::~UMPlayer()
{
}

bool UMPlayer::Init(const uint64 InPlayerId, const FString& InAccount)
{
	PlayerID = InPlayerId;
	Account = InAccount;
	return true;
}

void UMPlayer::Cleanup()
{
}

void UMPlayer::Online(UMGameSession* InSession)
{
	Session = InSession;
	Session->Player = this;
}

void UMPlayer::Offline(const UMGameSession* InSession)
{
	check(Session == InSession);

	//UE_LOG()

	Session = nullptr;

	PlayerData.LastOnlineTime = FDateTime::UtcNow().GetTicks();  // 更新离线时间
	
	Save();

	UMPlayerManager::Get()->DeletePlayer(this);
}

UMGameSession* UMPlayer::GetSession() const
{
	return Session;
}

FGuid UMPlayer::GetConnId() const
{
	if (Session)
		return Session->ID;
	else
	{
		return FGuid();
	}
}

void UMPlayer::SendToMe(const FGameMessage& InMessage) const
{
	if (!Session)
		return;
		
	Session->Send(InMessage);
}

bool UMPlayer::CreateRole(const FCreateRoleParams& Params)
{
	// Todo 建议姓名合法性，参数合法性

	// 角色名已经存在
	const uint64 ID = GenerateUID(Params.RoleName);
	if (GetRoleDataRef(ID))
		return false;
	
	FRoleData Data;

	Data.RoleName = Params.RoleName;
	Data.Class = Params.Class;
	Data.Camp = Params.Camp;
	Data.Gender = Params.Gender;
	Data.Race = Params.Race;
	Data.Birth = Params.Birth;

	Data.ID = ID;
	Data.CreateDate = FDateTime::UtcNow().GetTicks();
	Data.Rank = 1;

	PlayerData.RoleData.Add(Data);

	MarkNeedSave();

	return true;
}

void UMPlayer::SetCurrentRole(const uint64 ID)
{
	CurrentRole = GetRoleDataRef(ID);
}

void UMPlayer::SetCurrentRole(const FString& Name)
{
	CurrentRole = GetRoleDataRef(Name);
}

void UMPlayer::Fill(TArray<FPreviewRoleData>& Out)
{
	Out.Init(FPreviewRoleData(), PlayerData.RoleData.Num());
	for (int32 i = 0; i < PlayerData.RoleData.Num(); i++)
	{
		RoleDataToPreview(PlayerData.RoleData[i], Out[i]);
	}
}

FRoleData* UMPlayer::GetRoleDataRef(const uint64 ID)
{
	for (FRoleData& Data : PlayerData.RoleData)
	{
		if (Data.ID == ID)
			return &Data;
	}

	return nullptr;
}

FRoleData* UMPlayer::GetRoleDataRef(const FString& Name)
{
	for (FRoleData& Data : PlayerData.RoleData)
	{
		if (Data.RoleName == Name)
			return &Data;
	}

	return nullptr;
}

bool UMPlayer::IsOnline() const
{
	return Session != nullptr;
}

bool UMPlayer::Load()
{
	FRedisOp::LoadPlayerData(GetPlayerID(), &PlayerData);

	// 没有数据就当作新号初始化
	if (PlayerData.ID == 0)
	{
		PlayerData.Account = Account;
		PlayerData.ID = PlayerID;
		PlayerData.CreateDate = FDateTime::Now().GetTicks();
		
		return true;
	}

	// Todo Load功能模块数据
	
	return true;
}

void UMPlayer::Save()
{
	UpdateLastWorldInfo();//更新最后的关卡数据
	
	// Todo 功能模块SaveData

	FRedisOp::SavePlayerData(GetPlayerID(), PlayerData);
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

void UMPlayer::UpdateLastWorldInfo()
{
}

void UMPlayer::MarkNeedSave()
{
	if (NeedSaveFlag > 5)
	{
		Save();
		NeedSaveFlag = 0;
	}
	else
	{
		NeedSaveFlag += 1;
	}
}

void UMPlayer::OnOnline()
{
}

void UMPlayer::OnOffline()
{
}
