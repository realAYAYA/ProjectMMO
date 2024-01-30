#include "Editor.h"
//#include "Interfaces/IMainFrameModule.h"

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
		BlueprintToolbar->Initialize();
		LevelToolbar->Initialize();

		// UE5 在关卡编辑器栏目中添加自定义按钮的方法
		// 参考UEditorUtilitySubsystem::HandleStartup()的调用过程
		/*IMainFrameModule& MainFrameModule = IMainFrameModule::Get();
		if (MainFrameModule.IsWindowInitialized())
		{
			MainFrameCreationFinished();
		}
		else
		{
			MainFrameModule.OnMainFrameCreationFinished().AddLambda([this](TSharedPtr<SWindow>, bool) { MainFrameCreationFinished(); });
		}*/
	}

	void PostPIEStarted(bool bIsSimulating)
	{
		bIsPIE = true; // raise PIE flag
	}

	void PrePIEEnded(bool bIsSimulating)
	{
		bIsPIE = false; // clear PIE flag
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
	FDelegateHandle OnEditorInitializedHandle;

	bool bIsPIE;
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMEditorModule, MEditor)
