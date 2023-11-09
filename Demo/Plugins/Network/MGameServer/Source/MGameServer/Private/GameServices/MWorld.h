#pragma once

#include "CoreMinimal.h"

#include "MWorld.generated.h"

class UMGameSession;

UCLASS()
class UMWorld : public UObject
{
	GENERATED_BODY()
	
public:
	
	virtual ~UMWorld() override;

	bool Init(int64 InUserId, const FString& InAccount, int64 InRoleId);
	void Cleanup();

	void Online(UMGameSession* InSession);
	void Offline(UMGameSession* InSession);
	
	/** 是否在线 */
	bool IsOnline() const;

	UMGameSession* GetSession() const;
	FGuid GetConnId() const;
	
	/** 读档 */
	bool Load();
	
	/** 存档 */
	void Save();

private:

	void OnOnline();
	void OnOffline();

	const uint64 UserId = 0;
	// DSProcessHandle;

	UPROPERTY()
	UMGameSession* Session = nullptr;

	
};