// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "HyperlinkRichTextBlockDecorator.generated.h"

class UMRichTextBlockDecorator;

class FRichHyperLinkData
{
	
public:
	
	FRichHyperLinkData(UMRichTextBlockDecorator* Decorator, TMap<FString, FString> MetaData);

	FSimpleDelegate RichHyperLinkDelegate;
};

class FRichInlineHyperLinkDecorator : public FRichTextDecorator
{
	
public:
	FRichInlineHyperLinkDecorator(URichTextBlock* InOwner, UMRichTextBlockDecorator* InDecorator);

	virtual bool Supports(const FTextRunParseResults& RunInfo, const FString& Text) const override;

protected:

	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& DefaultTextStyle) const override;

	void ExplodeTextStyleInfo(TMap<FString, FString>& MetaData, FSlateFontInfo& OutFont, FLinearColor& OutFontColor) const;

	UMRichTextBlockDecorator* Decorator;

	mutable FHyperlinkStyle LinkStyle;
	mutable FSlateFontInfo DefaultFont;
	mutable FSlateColor DefaultColor;
	mutable TArray<FRichHyperLinkData> LinkData;
	mutable uint8 CurrentLinkIndex;
	mutable FString Content;
};

/**
 * 
 */
UCLASS()
class DEMO_API UMRichTextBlockDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	
	UMRichTextBlockDecorator(const FObjectInitializer& ObjectInitializer);

	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectM")
	FHyperlinkStyle Style;

	UFUNCTION(BlueprintNativeEvent, Category = "ProjectM")
	void OnClick(const TMap<FString, FString>& MetaData);

	UFUNCTION(BlueprintNativeEvent, Category = "ProjectM")
	void PreStyle(const TMap<FString, FString>& MetaData);

public:
	
	// Allows the Object to use BP_Functions
	virtual UWorld* GetWorld() const override;
	ULevel* GetLevel() const;
	
};
