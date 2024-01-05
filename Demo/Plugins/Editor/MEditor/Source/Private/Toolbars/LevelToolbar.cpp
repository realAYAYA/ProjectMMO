#include "LevelToolbar.h"
#include "LevelEditor.h"


void FLevelToolbar::Initialize()
{
	FMEditorToolbar::Initialize();
	
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	const auto ToolbarExtender = GetExtender(nullptr);
	//LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);// UE4.27


#define LOCTEXT_NAMESPACE "FMToolbarModule"

	FToolMenuEntry BlueprintEntry = FToolMenuEntry::InitComboButton(
					"ProjectM",
					FUIAction(),
					FOnGetContent::CreateStatic(&FMEditorToolbar::GenerateProjectMMenuContent, CommandList),
					LOCTEXT("MEditor_Label", "ProjectM Editor"),
					LOCTEXT("MEditor_ToolTip", "ProjectM Editor"),
					FSlateIcon("MEditorStyle", TEXT("MEditor.Icon1"))
				);
	
	BlueprintEntry.StyleNameOverride = "ProjectMToolbar";
	
	/*if (UToolMenu* UserToolBar = UToolMenus::Get()->FindMenu("LevelEditor.LevelEditorToolBar.User"))
	{
		FToolMenuSection& Section = UserToolBar->AddSection("ProjectM");
		Section.AddEntry(BlueprintEntry);
	}*/
	
	const auto ToolMenus = UToolMenus::Get();
	if (const auto LevelEditorToolbar = ToolMenus->FindMenu(FName("LevelEditor.LevelEditorToolbar.AssetsToolBar")))
	{
		LevelEditorToolbar->AddMenuEntry(FName("ProjectM"), BlueprintEntry);
		ToolMenus->RefreshAllWidgets();
	}

#undef LOCTEXT_NAMESPACE
	
}