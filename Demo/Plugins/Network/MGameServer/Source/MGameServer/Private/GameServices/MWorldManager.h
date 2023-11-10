#pragma once

#include "CoreMinimal.h"

#include "MMacros.h"
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

	UMWorld* CreateWorld();

	// 数据填充方法
	//void Fill(const UZRole* Role);

	UPROPERTY()
	UMWorld* MainWorld = nullptr;
};