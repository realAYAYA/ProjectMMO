#pragma once

#include "CoreMinimal.h"

#include "MMacros.h"
#include "MPlayer.h"
#include "MWorldManager.generated.h"

class UMWorld;
class UMPlayer;

UCLASS()
class UMWorldManager : public UObject
{
	GENERATED_BODY()
	
public:
	
	virtual ~UMWorldManager() override;

	M_UOBJECT_SINGLETON_DEFINE(UMWorldManager);

	void Init();
	void Shutdown();
	void Cleanup();

	void LoadData();
	void SaveData();

	bool Tick(float DeltaTime);

	// 生产基础大世界
	UMWorld* CreateWorld();

	// 临时测试用主关卡
	UPROPERTY()
	UMWorld* MainWorld = nullptr;

	// 创建地下城
	UMWorld* CreateDungeon(const UMPlayer* Player);

	// 数据填充方法
	//void Fill(const UZRole* Role);
};