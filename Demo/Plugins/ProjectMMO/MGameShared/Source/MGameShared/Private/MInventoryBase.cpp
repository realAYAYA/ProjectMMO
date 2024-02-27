#include "MInventoryBase.h"

UMInventoryBase::UMInventoryBase(const FObjectInitializer& ObjectInitializer)
{
}

UMInventoryBase::UMInventoryBase(FVTableHelper& Helper)
{
}

UMInventoryBase::~UMInventoryBase()
{
}

bool UMInventoryBase::Init()
{
	return false;
}

int32 UMInventoryBase::GetItemNumByCfgId(const int32 InCfgID)
{
	return 0;
}

bool UMInventoryBase::CheckItemNumByCfgId(const int32 InCfgID, const int32 InNum)
{
	return false;
}

int32 UMInventoryBase::GetInventoryNum() const
{
	return 0;
}

int32 UMInventoryBase::GetInventoryRemainingSpaceNum() const
{
	return 0;
}

void UMInventoryBase::InternalCleanUp()
{
}

void UMInventoryBase::InternalAddItem(UMItemBase* Item)
{
}

UMItemBase* UMInventoryBase::InternalGetItemById(int64 Id)
{
	return nullptr;
}

void UMInventoryBase::InternalForeachAllItems(const TFunction<bool(UMItemBase*)>& Func) const
{
}

int64 UMInventoryBase::GenerateItemUID()
{
	++SerialNum;
	if (SerialNum < 0)
		SerialNum = NormalItemIdBegin;
	
	while (AllItems.Find(SerialNum) != nullptr)
	{
		++SerialNum;
	}

	return SerialNum;
}
