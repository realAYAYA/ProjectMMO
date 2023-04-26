// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpComponent.h"

UPickUpComponent::UPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UPickUpComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
