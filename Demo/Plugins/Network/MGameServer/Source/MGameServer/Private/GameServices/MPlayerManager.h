#pragma once

#include "CoreMinimal.h"

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

	void LoadData();
	void SaveData();

	bool Tick(float DeltaTime);

	// 数据填充方法
	//void Fill(const UZRole* Role);
};