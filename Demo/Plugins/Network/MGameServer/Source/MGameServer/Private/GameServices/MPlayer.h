#pragma once

#include "CoreMinimal.h"
#include "GameMessage.h"
#include "MGameTypes.h"

#include "MPlayer.generated.h"

class UMWorld;
class UMGameSession;

UCLASS()
class UMPlayer : public UObject
{
	GENERATED_BODY()
	
public:
	
	virtual ~UMPlayer() override;

	bool Init(const uint64 InPlayerId, const FString& InAccount);
	void Cleanup();

	void Online(UMGameSession* InSession);
	void Offline(const UMGameSession* InSession);

	UMGameSession* GetSession() const;
	FGuid GetConnId() const;

	uint64 GetPlayerId() const { return PlayerId; }

	void SendToMe(const FGameMessage& InMessage) const;

	// ========================================================
	
	/** 是否在线 */
	bool IsOnline() const;
	
	/** 读档 */
	bool Load();
	
	/** 存档 */
	void Save();

	void SetCurWorldId(uint64 InWorldId);

	void EnterWorld(const FGuid& WorldID) const;
	uint64 GetCurrentWorldID() const;  // 获得当前所在场景Id
	uint32 GetCurrentWorldCfgID() const;  // 获得当前所在场景ID
	UMWorld* GetCurrentWorld() const;  // 获得当前所在场景对象
	void UpdateLastWorldInfo();
	
private:

	UPROPERTY()
	UMGameSession* Session = nullptr;
	
	void OnOnline();
	void OnOffline();

	uint64 PlayerId = 0;

	FString Account;
	
	FMPlayerData Data;
};
