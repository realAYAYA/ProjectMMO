#include "LevelToolbar.h"
#include "LevelEditor.h"

void FLevelToolbar::Initialize()
{
	FMEditorToolbar::Initialize();
	
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	const auto ToolbarExtender = GetExtender(nullptr);
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
}
