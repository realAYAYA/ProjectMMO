#include "MEditorStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BORDER_BRUSH(RelativePath, ...) FSlateBorderBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define TTF_FONT(RelativePath, ...) FSlateFontInfo(RootToContentDir(RelativePath, TEXT(".ttf")), __VA_ARGS__)
#define OTF_FONT(RelativePath, ...) FSlateFontInfo(RootToContentDir(RelativePath, TEXT(".otf")), __VA_ARGS__)

FMEditorStyle::FMEditorStyle()
	: FSlateStyleSet("MEditorStyle")
{
	//const FVector2D Icon40X40(40.0f, 40.0f);
	const FVector2D Icon48X48(48.0f, 48.0f);
	const FVector2D Icon64X64(64.0f, 64.0f);

	FSlateStyleSet::SetContentRoot(IPluginManager::Get().FindPlugin("MEditor")->GetBaseDir() / TEXT("Resources"));

	Set("MEditor.Icon1", new IMAGE_BRUSH("Icons/basket_empty", Icon48X48));
	Set("MEditor.Icon2", new IMAGE_BRUSH("Icons/basket_full", Icon48X48));
	
	Set("MEditor.StartGameService", new IMAGE_BRUSH("Icons/rocket", Icon64X64));
	Set("MEditor.StopGameService", new IMAGE_BRUSH("Icons/power", Icon64X64));
	
	FSlateStyleRegistry::RegisterSlateStyle(*this);
}

FMEditorStyle::~FMEditorStyle()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*this);
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
