#include "Editor.h"
#include "Interfaces/IMainFrameModule.h"

#include "Toolbars/LevelToolbar.h"
#include "Toolbars/BlueprintToolbar.h"

#include "MEditorStyle.h"
#include "MEditorCommands.h"


#define LOCTEXT_NAMESPACE "FMEditorModule"


class FMEditorModule : public IModuleInterface
{
	
public:
	
	FMEditorModule()
		: bIsPIE(false)
	{
	}

	virtual void StartupModule() override
	{
		Style = MakeShareable(new FMEditorStyle);

		FMEditorCommands::Register();

		// register delegates
		OnPostEngineInitHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &FMEditorModule::OnPostEngineInit);
		PostPIEStartedHandle = FEditorDelegates::PostPIEStarted.AddRaw(this, &FMEditorModule::PostPIEStarted);
		PrePIEEndedHandle = FEditorDelegates::PrePIEEnded.AddRaw(this, &FMEditorModule::PrePIEEnded);

		BlueprintToolbar = MakeShareable(new FBlueprintToolbar);
		LevelToolbar = MakeShareable(new FLevelToolbar);
	}

	virtual void ShutdownModule() override
	{
		FMEditorCommands::Unregister();

		// unregister delegates
		FCoreDelegates::OnPostEngineInit.Remove(OnPostEngineInitHandle);
		FEditorDelegates::PostPIEStarted.Remove(PostPIEStartedHandle);
		FEditorDelegates::PrePIEEnded.Remove(PrePIEEndedHandle);
	}

private:
	
	void OnPostEngineInit()
	{
		LevelToolbar->Initialize();
		BlueprintToolbar->Initialize();

		IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
		MainFrameModule.OnMainFrameCreationFinished().AddRaw(this, &FMEditorModule::OnMainFrameCreationFinished);
	}

	void PostPIEStarted(bool bIsSimulating)
	{
		bIsPIE = true; // raise PIE flag
	}

	void PrePIEEnded(bool bIsSimulating)
	{
		bIsPIE = false; // clear PIE flag
	}

	void OnMainFrameCreationFinished(TSharedPtr<SWindow> InRootWindow, bool bIsNewProjectWindow)
	{
	}

	bool IsPIE() const
	{
		return bIsPIE;
	}

	TSharedPtr<FLevelToolbar> LevelToolbar;
	TSharedPtr<FBlueprintToolbar> BlueprintToolbar;

	TSharedPtr<ISlateStyle> Style;

	FDelegateHandle OnPostEngineInitHandle;
	FDelegateHandle PostPIEStartedHandle;
	FDelegateHandle PrePIEEndedHandle;

	bool bIsPIE;
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMEditorModule, MEditor)
