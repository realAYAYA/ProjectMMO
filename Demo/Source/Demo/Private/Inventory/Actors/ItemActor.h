// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MGameDefine.h"
#include "Inventory/Item.h"

#include "ItemActor.generated.h"

class UPickUpComponent;

UCLASS()
class AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ItemID, Category = "ProjectM")
	int32 CfgID = 0;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ItemState, Category = "ProjectM")
	EItemState ItemState = EItemState::None;
	
	UPROPERTY(EditDefaultsOnly, Category = ProjectM)
	TObjectPtr<UMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = ProjectM)
	TObjectPtr<UPickUpComponent> PickComponent;

	UFUNCTION()
	virtual void OnPickUp(class AMCharacter* InOwner);
	
	UFUNCTION()
	virtual void OnDropped();

	UFUNCTION()
	void OnTake(AMCharacter* InOwner);

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
	UItem* Item;

	/**
	 * @Netwrok Replicated
	 */
public:
	
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
private:

	UFUNCTION()
	void OnRep_ItemState();

	UFUNCTION()
	void OnRep_ItemID();
	
};
