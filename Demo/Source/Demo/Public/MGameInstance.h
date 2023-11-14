// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MGameInstance.generated.h"

class UGameRpcClient;
class AMPlayerState;
class UGameTables;
class UMSaveGame;

/**
 * 
 */
UCLASS()
class DEMO_API UMGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void SetLoginInfo(const FString& InUserID, const FString& InUserName);

	// Steam, Epic, PS4 : ID
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectM")
	FString UserID;
	
	// Steam, Epic, PS4 : USerName
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectM")
	FString UserName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectM")
	UMSaveGame* SaveGame;
	
	virtual void Init() override;

	virtual void BeginDestroy() override;

	static UMGameInstance* GetMGameInstance(const UWorld* World);

	// DS server

	// Todo 后期不需要蓝图调用该方法，DS专用
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void LoginServer();
	
	// DS服务器准备就绪
	UFUNCTION()
	void GetReady();
	
	// DS服务器准备就绪
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	bool IsReady() const { return bReady; }

	UGameRpcClient* GetClientRpcStub() const;

private:

	//WorldInfo;// 关卡/副本信息
	bool bReady = false;
};
