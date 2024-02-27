#pragma once
#include "MGameTypes.h"
#include "MInventoryBase.h"
#include "MPlayerInventory.generated.h"

/** 道具数据缓存*/
USTRUCT() 
struct FRefreshItems
{
	GENERATED_USTRUCT_BODY()

	// 添加或更改
	UPROPERTY()
	TArray<FInventoryData> AddOrModify;

	// 待删除道具
	UPROPERTY()
	TArray<FInventoryData> Junks;

	bool IsEmpty() const
	{
		return !(!AddOrModify.IsEmpty() || !Junks.IsEmpty());
	}
	
	void Clear()
	{
		AddOrModify.Empty();
		Junks.Empty();
	}
};

UCLASS()
class UPlayerInventory : public UMInventoryBase
{
	GENERATED_BODY()
	
public:


	void Tick(const float DeltaTime);

	void SendRefreshItemsCacheToMe();

	FRefreshItems RefreshItemsCache;
};
