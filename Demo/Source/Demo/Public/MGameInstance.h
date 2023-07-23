// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameTypes.h"
#include "Engine/GameInstance.h"
#include "MGameInstance.generated.h"

class UMGameTables;

/**
 * 
 */
UCLASS()
class DEMO_API UMGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category = "ProjectSS")
	void Login(){};

	UFUNCTION(BlueprintCallable, Category = "ProjectSS")
	void Offline(){};

	UFUNCTION(BlueprintCallable, Category = "ProjectSS")
	void LoginChatServer(){}

	// Todo 拍卖行服务器

	UFUNCTION()
	UMGameTables* GetMGameTables();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString CharacterName;

	UFUNCTION(BlueprintCallable, Category = "ProjectSS")
	FMPlayerData GetCharacterData() const { return *PlayerData; }

	UFUNCTION(BlueprintCallable, Category = "ProjectSS")
	FMUserData GetUserData() const { return UserData; }

private:

	FMUserData UserData;

	FMPlayerData* PlayerData;

	UPROPERTY()
	UMGameTables* GameTables;
};
