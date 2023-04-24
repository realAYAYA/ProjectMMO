// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameTypes.generated.h"

USTRUCT(BlueprintType)
struct FGameUserData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = ProjectSS)
	int64 UserID = 0;

	
};

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = GAS)
	TArray<TSubclassOf<class UGameplayEffect>> Effects;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> Abilities;
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = ProjectSS)
	int32 ID = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = ProjectSS)
	TSoftObjectPtr<class USkeletalMesh> SkeletalMesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = ProjectSS)
	TSoftObjectPtr<class UStaticMesh> StaticMesh;

	
};