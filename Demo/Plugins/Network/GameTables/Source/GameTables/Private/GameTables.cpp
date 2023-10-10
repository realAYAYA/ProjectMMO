#include "GameTables.h"

DEFINE_LOG_CATEGORY(LogGameTables);

#define INIT_TABLE(TableType, VarName) \
	do { \
		if (!VarName) { \
			VarName = NewObject<TableType>(); \
		} \
		if (VarName->Init()) { \
			UE_LOG(LogGameTables, Warning, TEXT("[GameTables] %s加载成功 {}"), *VarName->GetConfigFileName()); \
		} \
		else { \
			UE_LOG(LogGameTables, Error, TEXT("[GameTables] %s加载失败 {}"), *VarName->GetConfigFileName()); \
		} \
	} while (false);

bool UGameTables::Init()
{
	UE_LOG(LogGameTables, Display, TEXT("[GameTables] Init begin..."));
	
	//INIT_TABLE(UZQiCollectorConfigManager, QiCollector);
	
	// ==============================================================
	// 以下为手动配置，仅.JsonData文件需要手动配置
	
	/*{
		const FString FileName = GetGameDesignDataFullPath() / TEXT("CommonConstants.jsondata");
		bool bOk = LoadObjectFromJsonFile(FileName, &CommonConstants);
		if (bOk)
		{
			UE_LOG(LogGameTables, Display, TEXT("[ZGameTables] 加载成功 CommonConstants.jsondata"));
		}
		else
		{
			UE_LOG(LogGameTables, Error, TEXT("[ZGameTables] 加载失败 CommonConstants.jsondata"));
		}		
		check(bOk);
	}*/

	UE_LOG(LogGameTables, Display, TEXT("[GameTables] Init end."));
	
	return true;
}
