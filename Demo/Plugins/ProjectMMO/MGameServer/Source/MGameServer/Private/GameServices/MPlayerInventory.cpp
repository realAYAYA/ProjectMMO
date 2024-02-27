#include "MPlayerInventory.h"

void UPlayerInventory::Tick(const float DeltaTime)
{
	if (!RefreshItemsCache.IsEmpty())
	{
		SendRefreshItemsCacheToMe();
	}
}

void UPlayerInventory::SendRefreshItemsCacheToMe()
{
	// Todo 发送背包数据变动缓存
	RefreshItemsCache.Clear();
}
