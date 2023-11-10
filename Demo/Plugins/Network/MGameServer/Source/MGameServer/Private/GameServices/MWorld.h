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

	bool Init();
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
	bool Save();

	void Reset(); // 重置世界/副本

private:

	void OnOnline();
	void OnOffline();

	const uint64 WorldId = 0;
	// DSProcessHandle;

	UPROPERTY()
	UMGameSession* Session = nullptr;

	// Data;// 存储了世界/副本信息 以及某些进度
};