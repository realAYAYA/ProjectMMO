// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MGameDefine.h"
#include "Inventory/ItemBase.h"

#include "ItemActor.generated.h"

class UPickUpComponent;

UCLASS()
class AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	TObjectPtr<UMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	TObjectPtr<UPickUpComponent> PickComponent;

	virtual void Init();

	UFUNCTION()
	virtual void OnPickUp(AActor* InOwner);
	
	UFUNCTION()
	virtual void OnDropped();

	UFUNCTION()
	void OnTake(AActor* InOwner);

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UMeshComponent* GetMeshComponent() const { return MeshComponent; }

	UFUNCTION()
	AActor* GetItemOwner() const { return ItemOwner; }
	
protected:

	virtual void InitInternal();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
