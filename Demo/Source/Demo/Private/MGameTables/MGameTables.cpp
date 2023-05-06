// Fill out your copyright notice in the Description page of Project Settings.


#include "MGameTables/MGameTables.h"
#include "MGameTables/ItemTable.h"

DEFINE_LOG_CATEGORY(LogMGameTables);

#define INIT_TABLE(TableType, VarName) \
if (!VarName) { \
	VarName = NewObject<TableType>(); \
} \
if (VarName->Init()) { \
	UE_LOG(LogMGameTables, Warning, TEXT("[GameTables] %s加载成功 {}"), *VarName->GetConfigFileName()); \
} \
else { \
	UE_LOG(LogMGameTables, Error, TEXT("[GameTables] %s加载失败 {}"), *VarName->GetConfigFileName()); \
} \

bool UMGameTables::Init()
{
	INIT_TABLE(UItemConfigManager, Item)
	
	return true;
}
