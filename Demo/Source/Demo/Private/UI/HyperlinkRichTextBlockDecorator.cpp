// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HyperlinkRichTextBlockDecorator.h"

UWorld* UHyperlinkRichTextBlockDecorator::GetWorld() const
{
	// CDO objects do not belong to a world
	// If the actors outer is destroyed or unreachable we are shutting down and the world should be nullptr
	if (!HasAnyFlags(RF_ClassDefaultObject) && ensureMsgf(GetOuter(), TEXT("Actor: %s has a null OuterPrivate in AActor::GetWorld()"), *GetFullName())
		&& !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		if (const ULevel* Level = GetLevel())
		{
			return Level->OwningWorld;
		}
	}
	
	return nullptr;
}

ULevel* UHyperlinkRichTextBlockDecorator::GetLevel() const
{
	return GetTypedOuter<ULevel>();
}
