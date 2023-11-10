#pragma once

#include "CoreMinimal.h"
#include "GameMessage.h"

#include "MMacros.h"
#include "MPlayerManager.generated.h"

class UMPlayer;

UCLASS()
class UMPlayerManager : public UObject
{
	GENERATED_BODY()
	
public:
	
	virtual ~UMPlayerManager() override;

	M_UOBJECT_SINGLETON_DEFINE(UMPlayerManager);

	void Init();
	void Shutdown();
	void Cleanup();

	void Tick(float DeltaTime);

	UMPlayer* GetByPlayerId(const uint64 Id);

	UMPlayer* CreatePlayer(const uint64 InPlayerId, const FString& InAccount);
	void DeletePlayer(UMPlayer* InPlayer);
	void DeletePlayerById(const uint64 InPlayerId);

	void Foreach(const TFunction<bool(UMPlayer*)>& InFunc);

	void SendToAll(const FGameMessage& InMessage);

	// 数据填充方法
	//void Fill(const UZRole* Role);

private:

	bool AddPlayer(UMPlayer* InPlayer);
	
	void ProcessJunk();

	UPROPERTY()
	TArray<UMPlayer*> AllEntities;
	
	UPROPERTY()
	TMap<uint64, UMPlayer*> IndexEntities;

	UPROPERTY()
	TArray<UMPlayer*> Junks;  // 负责持有待删除的指针

	FDateTime LastProcessTime;
};