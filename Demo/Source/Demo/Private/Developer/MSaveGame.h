// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "MGameTypes.h"

#include "MSaveGame.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DEMO_API UMSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	/**
	 *	@see UGameplayStatics::CreateSaveGameObject
	 *	@see UGameplayStatics::SaveGameToSlot
	 *	@see UGameplayStatics::DoesSaveGameExist
	 *	@see UGameplayStatics::LoadGameFromSlot
	 *	@see UGameplayStatics::DeleteGameInSlot
	 */
	
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	bool CreateUser(const int64 InID, const FString& InName);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void RemoveUser(const int64 InID);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	FMUserData GetUserData(const int64 InID);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	FRoleData GetRoleData(const int64 InID, const FString& InName);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	bool UpdateRoleName(const int64 InID, const FString& OldName, const FString& NewName);
	
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	bool UpdateRole(const int64 InID, const FString& InName, const FRoleData& InData);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void RemoveRole(const int64 InID, const FString& InName);

	
private:
	
	int32 FindUser(const int64 InID);
	int32 FindUser(const FString& InName);
	
	int32 FindRole(const int64 InID, const FString& InName);
	int32 FindRole(const int32 Index, const FString& InName);
	
	UPROPERTY()
	TArray<FMUserData> UserData;// SteamID, SteamName
	
	// Todo 游戏进度
	
	// Todo 成就，收集

};