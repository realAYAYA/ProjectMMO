// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

//#include "Windows/AllowWindowsPlatformTypes.h"
//#include "mysql.h"
//#include "ImageUtils.h"

#include "MySQLBPLibrary.generated.h"

using namespace std;

USTRUCT(BlueprintType, Category = "MySql|Tables")
struct FMySQLDataTable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLDataTable")
	FString ColumnName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLDataTable")
	TArray<FString> ColumnData;
};

USTRUCT(BlueprintType, Category = "MySql|Tables")
struct FMySQLDataRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLDataRow")
	TArray<FString> RowData;
};

UCLASS()
class MYSQLPLUGIN_API UMySQLPluginBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void Mysql_Connection(
		FString Server,
		FString DBName,
		FString UserID,
		FString Password,
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void Mysql_Close();

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void SelectDataTableRows(
		FString FieldName,
		FString TableName,
		bool& IsSuccess,
		FString& ErrorMassage,
		TArray<FString>& TableHead,
		TArray<FMySQLDataRow>& Rows);

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void insertRow(
		FString TableName,
		FString Values,
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void deleteRow(
		FString TableName,
		FString Field,
		FString Value,
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void UpdateTable(
		FString TableName,
		FString Field,
		FString NewValue,
		FString FormerField,
		FString FormerValue,
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void UpdateBinaryFile(
	FString TableName,
		FString FieldName,
		FString ID,
		FString InFile, 
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	static void Selectimage(
		FString TableName,
		FString ID,
		int ImageIndex,
		bool& IsSuccess,
		FString& ErrorMassage,
		UTexture2D*& Texture);

	UFUNCTION(BlueprintCallable, Category = "Paths")
	static void GetFiles(FString InPath, FString FileForm, TArray<FString>& FileList);

private:
	
	static char* GetCharfromFString(FString Query);
	static wchar_t* GetWCharfromChar(const char* Input);
};