// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "MGameTypes.h"

#include "MPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API AMPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	AMPlayerState();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ProjectM")
	bool IsOnline() const { return UserData.UserID > 0; }

	UFUNCTION(BlueprintCallable, Category = "ProjectM", DisplayName = "GetUserID")
	int64 K2_GetUserID() const { return UserData.UserID; }

	uint64 GetUserID() const { return UserData.UserID; }
	
	UFUNCTION(BlueprintCallable, Category = "ProjectM")
	int32 GetRoleNum() const;
	
	const FMUserData& GetUserData() const { return UserData; }

	void SetUserData(const FMUserData& InData) { UserData = InData; }
	
protected:

	virtual void CopyProperties(APlayerState* PlayerState) override;
	
	virtual void Reset() override;

private:

	UPROPERTY()
	FMUserData UserData;
};