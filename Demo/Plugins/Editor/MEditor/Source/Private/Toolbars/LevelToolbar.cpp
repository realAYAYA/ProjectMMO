#include "LevelToolbar.h"
//#include "LevelEditor.h"

void FLevelToolbar::Initialize()
{
	FMEditorToolbar::Initialize();

	// UE4.27写法
	//FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	//const auto ToolbarExtender = GetExtender(nullptr);
	//LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);


#define LOCTEXT_NAMESPACE "FMToolbarModule"

	FToolMenuEntry BlueprintEntry = FToolMenuEntry::InitComboButton(
					"ProjectM",
					FUIAction(),
					FOnGetContent::CreateStatic(&FMEditorToolbar::GenerateProjectMMenuContent, CommandList),
					LOCTEXT("MEditor_Label", "ProjectM"),
					LOCTEXT("MEditor_ToolTip", "ProjectM Editor"),
					FSlateIcon("MEditorStyle", TEXT("MEditor.Icon1"))
				);
	
	BlueprintEntry.StyleNameOverride = "ProjectMToolbar";

	// 模仿引擎插件写法，UE官方提供了ExtendMenu方法给插件拓展Editor
	if (UToolMenu* UserToolBar = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.User"))
	{
		FToolMenuSection& Section = UserToolBar->AddSection("ProjectM");
		Section.AddEntry(BlueprintEntry);
	}

	// 模仿UEditorUtilitySubsystem的写法，在主界面启动之后生成按钮
	/*const auto ToolMenus = UToolMenus::Get();
	if (const auto LevelEditorToolbar = ToolMenus->FindMenu(FName("LevelEditor.LevelEditorToolbar.User")))
	{
		LevelEditorToolbar->AddMenuEntry(FName("ProjectM"), BlueprintEntry);
		ToolMenus->RefreshAllWidgets();
	}*/

#undef LOCTEXT_NAMESPACE
	
}