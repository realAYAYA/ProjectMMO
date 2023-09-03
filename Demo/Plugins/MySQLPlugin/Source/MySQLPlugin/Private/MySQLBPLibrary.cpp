// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "MySQLBPLibrary.h"

/*登录*/
void UMySQLPluginBPLibrary::Connection(
	const FString& Server,
	const FString& DBName,
	const FString& UserID,
	const FString& Password,
	bool& IsSuccess,
	FString& ErrorMassage)
{
	
}

void UMySQLPluginBPLibrary::Close()
{
	
}

/*查询表，返回表的行结构数组*/
void UMySQLPluginBPLibrary::SelectDataTableRows(
	const FString& FieldName,
	const FString& TableName,
	bool& IsSuccess,
	FString& ErrorMassage,
	TArray<FString>& TableHead,
	TArray<FMySQLDataRow>& Rows)
{
	
}

void UMySQLPluginBPLibrary::InsertRow(
		const FString& TableName,
		const FString& Values,
		bool& IsSuccess,
		FString& ErrorMassage)
{
	
}

void UMySQLPluginBPLibrary::DeleteRow(
	const FString& TableName,
	const FString& Field,
	const FString& Value,
	bool& IsSuccess,
	FString& ErrorMassage)
{
	
}

void UMySQLPluginBPLibrary::UpdateTable(
	const FString& TableName,
	const FString& Field,
	const FString& NewValue,
	const FString& FormerField,
	const FString& FormerValue,
	bool& IsSuccess,
	FString& ErrorMassage)
{
	
}

void UMySQLPluginBPLibrary::UpdateBinaryFile(
	const FString& TableName,
	const FString& FieldName,
	const FString& ID,
	const FString& InFilePath, 
	bool& IsSuccess,
	FString& ErrorMassage)
{

}

void UMySQLPluginBPLibrary::SelectImage(
	const FString& TableName,
	const FString& ID,
	const int ImageIndex,
	bool& IsSuccess,
	FString& ErrorMassage,
	UTexture2D*& Texture)
{
	
}

//查找指定目录下指定扩展名的文件
void UMySQLPluginBPLibrary::GetFiles(const FString& InPath, const FString& FileForm, TArray<FString>&FileList)
{
	const FString SearchedFiles = InPath + "/" + FileForm;
	
	IFileManager::Get().FindFiles(FileList, *SearchedFiles, true, false);
}




