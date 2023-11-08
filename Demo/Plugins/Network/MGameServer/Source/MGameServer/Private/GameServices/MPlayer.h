#pragma once

#include "CoreMinimal.h"
#include "GameMessage.h"

#include "MPlayer.generated.h"

class UMWorld;
class UMGameSession;

UCLASS()
class UMPlayer : public UObject
{
	GENERATED_BODY()
	
public:
	
	virtual ~UMPlayer() override;

	bool Init(int64 InUserId, const FString& InAccount, int64 InRoleId);
	void Cleanup();

	void Online(UMGameSession* InSession);
	void Offline(UMGameSession* InSession);

	UMGameSession* GetSession();
	FGuid GetConnId() const;

	void SendToMe(const FGameMessage& InMessage) const;

	// ========================================================
	
	/** 是否在线 */
	bool IsOnline() const;
	
	/** 读档 */
	bool Load();
	
	/** 存档 */
	void Save();

	void SetCurWorldId(uint64 InWorldId);
	
	int64 GetCurrentWorldID() const;  // 获得当前所在场景Id
	int32 GetCurrentWorldCfgID() const;  // 获得当前所在场景Id
	UMWorld* GetCurrentWorld() const;  // 获得当前所在场景对象
	
private:

	const uint64 UserId = 0;
	const FString Account;

	UPROPERTY()
	UMGameSession* Session = nullptr;
	
	void OnOnline();
	void OnOffline();
	
};
