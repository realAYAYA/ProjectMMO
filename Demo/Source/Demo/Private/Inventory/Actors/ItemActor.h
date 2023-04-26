// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameDefine.h"
#include "GameFramework/Actor.h"
#include "Inventory/ItemBase.h"
#include "ItemActor.generated.h"

UCLASS()
class AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	UFUNCTION()
	AActor* GetItemOwner() const { return ItemOwner; }

	UFUNCTION(BlueprintPure, Category = ProjectSS)
	UMeshComponent* GetMeshComponent() const { return MeshComponent; }

	virtual void Init();

	UFUNCTION()
	virtual void OnPickUp(AActor* InOwner);
	
	UFUNCTION()
	virtual void OnDropped();

	UFUNCTION()
	void OnTake(AActor* InOwner);

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	class UPickUpComponent* PickComponent;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
protected:

	virtual void InitInternal();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UMeshComponent> MeshComponent;

private:
	UPROPERTY()
	AActor* ItemOwner = nullptr;

	UPROPERTY()
	UItemBase* ItemConfig;

	/**
	 * @Netwrok Online State based On UE DS
	 */
protected:
	
	UPROPERTY(ReplicatedUsing = OnRep_ItemState)
	TEnumAsByte<EItemState> ItemState = EItemState::None;
	
private:

	UFUNCTION()
	void OnRep_ItemState();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ItemID)
	int32 ItemID = 0;

	UFUNCTION()
	void OnRep_ItemID();
	
};
