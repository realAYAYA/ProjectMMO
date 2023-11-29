// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MGameInstance.generated.h"

class UGameRpcClient;
class AMPlayerState;
class UGameTables;
class UMSaveGame;

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnLoginResult, const ELoginGameRetCode, RetCode, const TArray<FPreviewRoleData>&, PreviewData);// Todo 角色预览数据

/**
 * 
 */
UCLASS()
class DEMO_API UMGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;

	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void SetLoginInfo(const FString& InUserID, const FString& InUserName);

	// 请求角色数据
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "Login")
	void K2_Login(const FString& Account, const FOnLoginResult& Callback);

	uint64 GetPlayerID() const { return PlayerID; }

	// Steam, Epic, PS4 : ID
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectM")
	FString UserID;
	
	// Steam, Epic, PS4 : USerName
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectM")
	FString UserName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectM")
	UMSaveGame* SaveGame;

	static UMGameInstance* GetMGameInstance(const UWorld* World);

	void LoadLocalSave();

private:

	uint64 PlayerID = 0;
	
	// DS Only
	
public:
	
	void LoginServer();
	void CloseConnection();

	UFUNCTION()
	void OnDisConnected();

	int32 ReconnectTime = 0;// 重试连接次数（超过5次失败就执行一些其它行为）
	
	// DS服务器准备就绪
	UFUNCTION()
	void GetReady(bool Success);
	
	// DS服务器准备就绪
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	bool IsReady() const;

	UGameRpcClient* GetClientRpcStub() const;

private:

	//WorldInfo;// 关卡/副本信息
	bool bReady = false;
};
