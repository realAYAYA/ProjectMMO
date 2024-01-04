#include "LevelToolbar.h"
#include "LevelEditor.h"

void FLevelToolbar::Initialize()
{
	FMEditorToolbar::Initialize();
	
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	const auto ToolbarExtender = GetExtender(nullptr);
	//LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	auto& ExtenderDelegates = LevelEditorModule.GetMenuExtensibilityManager()->GetExtenderDelegates();
	ExtenderDelegates.Add(
		FAssetEditorExtender::CreateLambda
		([&](const TSharedRef<FUICommandList>&, const TArray<UObject*>& ContextSensitiveObjects)
		{
			const auto TargetObject = ContextSensitiveObjects.Num() < 1 ? nullptr : Cast<UBlueprint>(ContextSensitiveObjects[0]);
			return GetExtender(TargetObject);
		}));
}
