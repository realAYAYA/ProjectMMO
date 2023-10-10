#pragma once

#include "CoreMinimal.h"

#include "IWebSocket.h"
#include "MGameClientSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnConnectServer, bool, Ok);
DECLARE_DYNAMIC_DELEGATE(FOnDisConnectServer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnErrorCallback, const FString&, Error);

UCLASS()
class UMGameClientSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

public:

	// Todo 测试用的显示调用
	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "CreateSocket")
	void K2_CreateSocket(const FString& ServerURL, const FString& ServerProtocol, const FOnConnectServer& Callback);

	void CreateSocket(const FString& ServerURL, const FString& ServerProtocol);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void CloseSocket(const FOnDisConnectServer& Callback);

	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	void Send(const TArray<uint8>& Data) const;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM")
	FOnErrorCallback OnErrorCallback;

protected:
	
	void OnConnected() const;
	void OnConnectionError(const FString& Error) const;
	void OnClosed(const int32 StatusCode, const FString& Reason, const bool bWasClean);
	void OnMessage(const FString& Message);
	void OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);
	void OnBinaryMessage(const void* Data, SIZE_T Size, bool bIsLastFragment);
	void OnMessageSent(const FString& Message);

private:

	TSharedPtr<IWebSocket> Socket;
	
	FOnConnectServer OnConnectedServer;

	FOnDisConnectServer OnDisConnectedServer;
};
