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
	bool CreateUser(const FString& InID, const FString& InName);

	bool CreateUser(const FMUserData& InData);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void RemoveUser(const FString& InID);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	FMUserData FindUserData(const FString& InID);
	
	FMUserData* FindUserDataRef(const FString& InID);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	FRoleData GetRoleData(const FString& InID, const FString& InName);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	bool UpdateRoleName(const FString& InID, const FString& OldName, const FString& NewName);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	bool CreateRole(const FString& InID, const FCreateRoleParams& InParams);
	
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	bool UpdateRole(const FString& InID, const FString& InName, const FRoleData& InData);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void RemoveRole(const FString& InID, const FString& InName);

	
private:
	
	int32 FindUserIndex(const FString& InID);
	
	int32 FindRoleIndex(const FString& InID, const FString& InName);
	int32 FindRoleIndex(const int32 Index, const FString& InName);
	
	UPROPERTY()
	TArray<FMUserData> UserData;// SteamID, SteamName
	
	// Todo 游戏进度
	
	// Todo 成就，收集

};