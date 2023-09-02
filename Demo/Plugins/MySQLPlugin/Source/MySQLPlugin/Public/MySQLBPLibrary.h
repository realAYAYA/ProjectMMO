// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MySQLBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MYSQLPLUGIN_API UMySQLPluginBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void Connection(
		const FString& Server,
		const FString& DBName,
		const FString& UserID,
		const FString& Password,
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void Close();

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void SelectDataTableRows(
		const FString& FieldName,
		const FString& TableName,
		bool& IsSuccess,
		FString& ErrorMassage,
		TArray<FString>& TableHead,
		TArray<FMySQLDataRow>& Rows);

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void InsertRow(
		const FString& TableName,
		const FString& Values,
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void DeleteRow(
		const FString& TableName,
		const FString& Field,
		const FString& Value,
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void UpdateTable(
		const FString& TableName,
		const FString& Field,
		const FString& NewValue,
		const FString& FormerField,
		const FString& FormerValue,
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void UpdateBinaryFile(
		const FString& TableName,
		const FString& FieldName,
		const FString& ID,
		const FString& InFilePath, 
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void SelectImage(
		const FString& TableName,
		const FString& ID,
		const int ImageIndex,
		bool& IsSuccess,
		FString& ErrorMassage,
		UTexture2D*& Texture);

	UFUNCTION(BlueprintCallable, Category = "Paths")
	static void GetFiles(const FString& InPath, const FString& FileForm, TArray<FString>& FileList);
};