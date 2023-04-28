// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameTypes.generated.h"

USTRUCT(BlueprintType) 
struct FGamePlayerData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int64 UserID = 0;
	
};

USTRUCT(BlueprintType)
struct FGameUserData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int64 UserID = 0;

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	FGamePlayerData PlayerData;
	
};


USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TArray<TSubclassOf<class UGameplayEffect>> Effects;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> Abilities;
};

UCLASS()
class UItemData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = ProjectSS)
	int32 ID = 0;
	
};

UCLASS()
class UGearData : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = ProjectSS)
	TSubclassOf<UGameplayEffect> DamageEffect;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = ProjectSS)
	TSoftObjectPtr<class USkeletalMesh> SkeletalMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = ProjectSS)
	TSoftObjectPtr<class UStaticMesh> StaticMesh;
};