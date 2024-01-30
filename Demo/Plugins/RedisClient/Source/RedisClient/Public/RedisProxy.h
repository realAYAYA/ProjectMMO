#pragma once

#include "Modules/ModuleManager.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RedisProxy.generated.h"

class FRedisClient;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class REDISCLIENT_API URedisProxy : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Redis")
	FString IP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Redis")
	FString Password;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Redis")
	int32 Port = 0;
	
	URedisProxy();

	UFUNCTION(BlueprintCallable, Category = "Redis")
	bool ConnectNow();
	
	UFUNCTION(BlueprintCallable, Category = "Redis")
	bool Connect(const FString& InIP, const FString& InPassword, const int32 InPort = 6379);

	UFUNCTION(BlueprintCallable, Category = "Redis")
	void Disconnect();

	/** Please check the FRedisClient*/
	UFUNCTION(BlueprintCallable, Category = "Redis")
	bool ExecuteCmd(const FString& Cmd);

protected:

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

private:
	
	TUniquePtr<FRedisClient> RedisClient;

	FDateTime LastCheckTime;
};

