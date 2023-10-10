#pragma once

#include "Modules/ModuleManager.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RedisProxy.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class REDISCLIENT_API URedisProxy : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Redis")
	FString IP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Redis")
	FString Password;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Redis")
	int32 Port;
	
	URedisProxy();

	UFUNCTION(BlueprintCallable, Category = "Redis")
	bool Connect(const FString& InIP, const FString& InPassword, const int32 InPort);

	UFUNCTION(BlueprintCallable, Category = "Redis")
	bool Disconnect();

	UFUNCTION(BlueprintCallable, Category = "Redis")
	bool ExecuteCmd(const FString& Cmd);

protected:

	//virtual void Tick()// Todo tick重连，配置重连时间间隔

private:
	
	TUniquePtr<class FRedisClient> RedisClient;

	FDateTime LastTime;
};