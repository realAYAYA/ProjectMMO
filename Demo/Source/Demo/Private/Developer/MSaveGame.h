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
class DEMO_API USaveGameMain : public USaveGame
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
	bool CreateUser(const FString& InUserID, const FString& InName);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	FMUserData RemoveUser(const FString& InUserID);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	FMUserData GetUserData(const FString& InUserID);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	FMCharacterData GetCharacter(const FString& InUserID, const FString& InName);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	bool UpdateCharacterName(const FString& InUserID, const FString& OldName, const FString& NewName);
	
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	bool UpdateCharacter(const FString& InUserID, const FString& InName, const FMCharacterData& InData);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void RemoveCharacter(const FString& InUserID, const FString& InName);

private:
	
	UPROPERTY()
	TArray<FMUserData> UserData;// SteamID, SteamName
	
	// Todo 游戏进度
	
	// Todo 成就，收集

};