// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "HyperlinkRichTextBlockDecorator.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API UHyperlinkRichTextBlockDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

	
public:
	
	// Allows the Object to use BP_Functions
	virtual UWorld* GetWorld() const override;
	ULevel* GetLevel() const;
	
};
