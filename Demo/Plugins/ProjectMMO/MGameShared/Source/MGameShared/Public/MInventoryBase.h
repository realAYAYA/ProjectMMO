#pragma once
#include "MInventoryBase.generated.h"

constexpr int64 NormalItemIdBegin = 10000000;

class UMItemBase;

UCLASS()
class MGAMESHARED_API UMInventoryBase : public UObject
{
	GENERATED_BODY()
	
public:

	UMInventoryBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	UMInventoryBase(FVTableHelper& Helper);
	virtual ~UMInventoryBase() override;

	virtual bool Init();

	/** 通过道具配置ID获取道具数量 */
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	int32 GetItemNumByCfgId(const int32 InCfgID);

	/* 检查包裹是否有足够数量的道具 */
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	bool CheckItemNumByCfgId(const int32 InCfgID, const int32 InNum);

	/** 获取包裹中道具数量 */
	UFUNCTION(BlueprintPure, Category="IdleZ")
	int32 GetInventoryNum() const;

	/* 获取包裹剩余空间 */
	UFUNCTION(BlueprintPure, Category="IdleZ")
	int32 GetInventoryRemainingSpaceNum() const;

protected:

	void InternalCleanUp();

	void InternalAddItem(UMItemBase* Item);

	// 通过道具唯一ID获取道具
	UMItemBase* InternalGetItemById(int64 Id);
	
	// 遍历所有道具
	void InternalForeachAllItems(const TFunction<bool(UMItemBase*)>& Func) const;

private:
	
	UPROPERTY()
	TMap<int64, UMItemBase*> AllItems;

	//UPROPERTY()
	//UMPlayer* Owner;// Todo 放在子类中比较合适
	
protected:

	int64 GenerateItemUID();

private:

	int64 SerialNum = NormalItemIdBegin;
};
