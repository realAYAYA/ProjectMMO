#pragma once

#include "CoreMinimal.h"

#include "NetFwd.h"
#include "RpcManager.h"

#include "MGameClientSubsystem.generated.h"

class UGameRpcClient;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnConnectServer, bool, Ok);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnErrorCallback, const FString&, Error);

UCLASS()
class MGAMECLIENT_API UMGameClientSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	UPROPERTY(BlueprintReadOnly, Category = "ProjcetM")
	UGameRpcClient* GameRpcClient;

	// Todo Test for Blueprint
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "CreateSocket")
	void K2_CreateSocket(const FString& ServerURL, const FString& ServerProtocol, const FOnConnectServer& Callback);
	
	void CreateSocket(const FString& ServerURL, const FString& ServerProtocol);
	FOnConnectServer OnConnectedCallback;

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void CloseSocket();

	// Suggest: Establishing connection first before binding this callback
	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnErrorCallback OnErrorCallback;

	bool IsConnected() const { return Connection != nullptr; }

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

	FClientRpcManager RpcManager;
};
