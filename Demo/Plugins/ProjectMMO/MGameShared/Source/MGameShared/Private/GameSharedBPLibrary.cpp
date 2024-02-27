// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameSharedBPLibrary.h"
#include "MGameShared.h"

UGameSharedBPLibrary::UGameSharedBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UGameSharedBPLibrary::GameSharedSampleFunction(float Param)
{
	return -1;
}

