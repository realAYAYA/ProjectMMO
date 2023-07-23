// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HyperlinkRichTextBlockDecorator.h"

#include "Framework/Text/SlateHyperlinkRun.h"
#include "Widgets/Input/SRichTextHyperlink.h"

FRichHyperLinkData::FRichHyperLinkData(UMRichTextBlockDecorator* Decorator, TMap<FString, FString> MetaData)
{
	RichHyperLinkDelegate.BindLambda([=]()
	{
		if (Decorator)
			Decorator->OnClick(MetaData);
	});
}

FRichInlineHyperLinkDecorator::FRichInlineHyperLinkDecorator(URichTextBlock* InOwner, UMRichTextBlockDecorator* InDecorator)
	: FRichTextDecorator(InOwner)
{
	Decorator = InDecorator;

	CurrentLinkIndex = 0;
}

bool FRichInlineHyperLinkDecorator::Supports(const FTextRunParseResults& RunInfo, const FString& Text) const
{
	if (RunInfo.Name == TEXT("a"))
	{
		TMap<FString, FString> MetaData;
		TArray<FString> Keys;
		RunInfo.MetaData.GetKeys(Keys);

		for (int32 i = 0; i < Keys.Num(); i++)
		{
			const FTextRange& CurrentRange = RunInfo.MetaData[Keys[i]];
			FString ValueString = Text.Mid(CurrentRange.BeginIndex, CurrentRange.EndIndex - CurrentRange.BeginIndex);
			MetaData.Add(Keys[i], ValueString);
		}

		Decorator->PreStyle(MetaData);
		LinkStyle = Decorator->Style;
		DefaultFont = LinkStyle.TextStyle.Font;
		DefaultColor = LinkStyle.TextStyle.ColorAndOpacity;

		if (MetaData.Contains("font") || MetaData.Contains("size") || MetaData.Contains("style") || MetaData.Contains("color"))
		{
			FSlateFontInfo OutFont;
			FLinearColor OutColor;
			ExplodeTextStyleInfo(MetaData, OutFont, OutColor);
			LinkStyle.TextStyle.SetFont(OutFont);
			LinkStyle.TextStyle.ColorAndOpacity = FSlateColor(OutColor);
		}
		else
		{
			LinkStyle.TextStyle.SetFont(DefaultFont);
			LinkStyle.TextStyle.ColorAndOpacity = DefaultColor;
		}

		LinkData.Add(FRichHyperLinkData(Decorator, MetaData));
		CurrentLinkIndex = LinkData.Num() - 1;
		Content = Text.Mid(RunInfo.ContentRange.BeginIndex, RunInfo.ContentRange.EndIndex - RunInfo.ContentRange.BeginIndex);

		return true;
	}
	
	return false;
}

TSharedPtr<SWidget> FRichInlineHyperLinkDecorator::CreateDecoratorWidget(const FTextRunInfo& RunInfo,
	const FTextBlockStyle& DefaultTextStyle) const
{
	const TSharedPtr<FSlateHyperlinkRun::FWidgetViewModel> Model = MakeShareable(new FSlateHyperlinkRun::FWidgetViewModel);
 
	TSharedPtr<SRichTextHyperlink> Link = SNew(SRichTextHyperlink, Model.ToSharedRef())
											  .Text(FText::FromString(Content))
											  .Style(&LinkStyle)
											  .OnNavigate(LinkData[CurrentLinkIndex].RichHyperLinkDelegate);
 
	return Link;
}

void FRichInlineHyperLinkDecorator::ExplodeTextStyleInfo(
	TMap<FString, FString>& MetaData,
	FSlateFontInfo& OutFont,
	FLinearColor& OutFontColor) const
{
	OutFont = DefaultFont;
 
	const FString* const FontFamilyString = MetaData.Find(TEXT("font"));
	const FString* const FontSizeString = MetaData.Find(TEXT("size"));
	const FString* const FontStyleString = MetaData.Find(TEXT("style"));
	const FString* const FontColorString = MetaData.Find(TEXT("color"));
 
	if (FontFamilyString)
	{
		const FSoftObjectPath Font(**FontFamilyString);
		if (const UObject* FontAsset = Font.TryLoad())
		{
			OutFont.FontObject = FontAsset;
		}
		MetaData.Remove("font");
	}
 
	if (FontSizeString)
	{
		OutFont.Size = static_cast<uint8>(FPlatformString::Atoi(**FontSizeString));
		MetaData.Remove("size");
	}
 
	if (FontStyleString)
	{
		OutFont.TypefaceFontName = FName(**FontStyleString);
		MetaData.Remove("style");
	}
 
	OutFontColor = DefaultColor.GetSpecifiedColor();
	if (FontColorString)
	{
		const FString& FontColorStringRef = *FontColorString;
 
		// Is Hex color?
		if (!FontColorStringRef.IsEmpty() && FontColorStringRef[0] == TCHAR('#'))
		{
			OutFontColor = FLinearColor(FColor::FromHex(FontColorStringRef));
		}
		else if (OutFontColor.InitFromString(*FontColorString))
		{
			// Initialized
		}
		else
		{
			OutFontColor = DefaultColor.GetSpecifiedColor();
		}
		MetaData.Remove("color");
	}
}

UMRichTextBlockDecorator::UMRichTextBlockDecorator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

TSharedPtr<ITextDecorator> UMRichTextBlockDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FRichInlineHyperLinkDecorator(InOwner, this));
}

void UMRichTextBlockDecorator::PreStyle_Implementation(const TMap<FString, FString>& MetaData)
{
	
}

void UMRichTextBlockDecorator::OnClick_Implementation(const TMap<FString, FString>& MetaData)
{
	
}

UWorld* UMRichTextBlockDecorator::GetWorld() const
{
	// CDO objects do not belong to a world
	// If the actors outer is destroyed or unreachable we are shutting down and the world should be nullptr
	if (!HasAnyFlags(RF_ClassDefaultObject) && ensureMsgf(GetOuter(), TEXT("Actor: %s has a null OuterPrivate in AActor::GetWorld()"), *GetFullName())
		&& !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		if (const ULevel* Level = GetLevel())
			return Level->OwningWorld.Get();
		
		const UObject* Outer = GetOuter();
		while (Outer)
		{
			if (UWorld* World = Outer->GetWorld())
				return World;

			Outer = Outer->GetOuter();
		}
	}
	
	return nullptr;
}

ULevel* UMRichTextBlockDecorator::GetLevel() const
{
	return GetTypedOuter<ULevel>();
}

