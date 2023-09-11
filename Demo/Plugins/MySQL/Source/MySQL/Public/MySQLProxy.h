// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "mysql.h"

#include "MySQLProxy.generated.h"

USTRUCT(BlueprintType, Category = "MySQL")
struct FMySQLDataTable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLDataTable")
	FString ColumnName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLDataTable")
	TArray<FString> ColumnData;
};

USTRUCT(BlueprintType, Category = "MySQL")
struct FMySQLDataRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLDataRow")
	TArray<FString> RowData;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MYSQL_API UMySQLProxy : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "MySQL")
	int32 OutTime = 3;//连接超时设定

	UPROPERTY(EditAnywhere, Category = "MySQL")
	int32 ReConnection = 1;//自动重连

	UFUNCTION(BlueprintCallable, Category = "MYSQL")
	void Connect(
		const FString& Server,
		const FString& DBName,
		const FString& UserID,
		const FString& Password,
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MySQL")
	void DisConnect();

	UFUNCTION(BlueprintCallable, Category = "MySQL")
	void SelectDataTableRows(
		const FString& FieldName,
		const FString& TableName,
		bool& IsSuccess,
		FString& ErrorMassage,
		TArray<FString>& TableHead,
		TArray<FMySQLDataRow>& Rows);

	UFUNCTION(BlueprintCallable, Category = "MySQL")
	void InsertRow(
		const FString& TableName,
		const FString& Values,
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MySQL")
	void DeleteRow(
		const FString& TableName,
		const FString& Field,
		const FString& Value,
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MySQL")
	void UpdateTable(
		const FString& TableName,
		const FString& Field,
		const FString& NewValue,
		const FString& FormerField,
		const FString& FormerValue,
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MySQL")
	void UpdateBinaryFile(
		const FString& TableName,
		const FString& FieldName,
		const FString& ID,
		const FString& InFilePath, 
		bool& IsSuccess,
		FString& ErrorMassage);

	UFUNCTION(BlueprintCallable, Category = "MySQL")
	void SelectImage(
		const FString& TableName,
		const FString& ID,
		const int ImageIndex,
		bool& IsSuccess,
		FString& ErrorMassage,
		UTexture2D*& Texture);

	UMySQLProxy();

	virtual void BeginDestroy() override;
	
private:

	MYSQL MySQL;// MySQL上下文
};