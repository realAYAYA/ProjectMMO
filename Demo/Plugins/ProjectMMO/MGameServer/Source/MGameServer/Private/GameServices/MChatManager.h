#pragma once

#include "CoreMinimal.h"

#include "MMacros.h"
#include "MChatManager.generated.h"

class UMPlayer;

UCLASS()
class UMChatManager : public UObject
{
	GENERATED_BODY()
	
public:
	
	virtual ~UMChatManager() override;

	M_UOBJECT_SINGLETON_DEFINE(UMChatManager);

	void Init();
	void Shutdown();
	void Cleanup();

	void LoadData();
	void SaveData();

	bool Tick(float DeltaTime);

	// 数据填充方法
	//void Fill(const UZRole* Role);
};