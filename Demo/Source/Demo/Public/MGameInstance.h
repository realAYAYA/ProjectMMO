// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameTypes.h"
#include "Engine/GameInstance.h"
#include "MGameInstance.generated.h"

class AMPlayerState;
class UMGameTables;
class UGameRpc;

/**
 * 
 */
UCLASS()
class DEMO_API UMGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	/** Network */
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "ProjectM", meta = (WorldContext = "WorldContextObject"))
	const UGameRpc* GetGameRpc() const;

	// Todo 拍卖行服务器

	UFUNCTION()
	UMGameTables* GetMGameTables();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString CharacterName;

public:
	virtual void Init() override;

	static UMGameInstance* GetMGameInstance(const UWorld* World);
	
private:
	UPROPERTY()
	UMGameTables* GameTables;
};
