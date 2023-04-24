// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGameDefine.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

UCLASS()
class AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	virtual void Init();

	UPROPERTY()
	class UMeshComponent* MeshComponent;

	UFUNCTION()
	virtual void OnDropped();

	UFUNCTION()
	void OnTake(AActor* InOwner);

	UPROPERTY(EditDefaultsOnly, Category = ProjectSS)
	class USphereComponent* SphereComponent;

	UFUNCTION()
	AActor* GetItemOwner() const { return Owner; }

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	virtual void InitInternal();

	/**
	 * @Netwrok Online State based On UE DS 
	 */
	UPROPERTY(ReplicatedUsing = OnRep_ItemState)
	TEnumAsByte<EItemState> ItemState = EItemState::None;
	
	UFUNCTION()
	void OnRep_ItemState();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	AActor* Owner = nullptr;
};
