#pragma once

#include "CoreMinimal.h"

#include "NetFwd.h"
#include "RpcManager.h"

#include "MGameClientSubsystem.generated.h"

class UGameRpcClient;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnConnectServer, bool, Ok);
DECLARE_DYNAMIC_DELEGATE(FOnDisConnectServer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnErrorCallback, const FString&, Error);

UCLASS()
class UMGameClientSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	UPROPERTY(BlueprintReadOnly, Category = "ProjcetM")
	UGameRpcClient* GameRpcClient;

	// Todo 测试用的显示调用
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "CreateSocket")
	void K2_CreateSocket(const FString& ServerURL, const FString& ServerProtocol, const FOnConnectServer& Callback);

	void CreateSocket(const FString& ServerURL, const FString& ServerProtocol);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void CloseSocket(const FOnDisConnectServer& Callback);

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnErrorCallback OnErrorCallback;

protected:
	
	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(const int32 StatusCode, const FString& Reason, const bool bWasClean);
	//void OnMessage(const FString& Message);
	void OnRawMessage(const void* InData, SIZE_T Size, SIZE_T BytesRemaining);
	//void OnBinaryMessage(const void* InData, SIZE_T Size, bool bIsLastFragment);
	//void OnMessageSent(const FString& Message);

private:

	FConnectionPtr Connection;
	
	FOnConnectServer OnConnectedServer;

	FOnDisConnectServer OnDisConnectedServer;

	FClientRpcManager RpcManager;
};
