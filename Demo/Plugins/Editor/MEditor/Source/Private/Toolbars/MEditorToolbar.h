#pragma once

class FMEditorToolbar
{
	
public:
	
	virtual ~FMEditorToolbar() = default;
	FMEditorToolbar();

	TSharedRef<FUICommandList> GetCommandList() const
	{
		return CommandList;
	}

	virtual void Initialize();

	void StartGameService_Executed();
	void StopGameService_Executed();
	//void StartLoadTesting_Executed();
	//void StopLoadTesting_Executed();
	void ShowGddInFileManager_Executed();
	void ShowExcelInFileManager_Executed();
	//void UpdateGdd_Executed();
	//void ReloadGdd_Executed();

protected:
	
	virtual void BindCommands();

	static TSharedRef< SWidget > GenerateProjectMMenuContent(TSharedRef<FUICommandList> InCommandList);

	void BuildToolbar(FToolBarBuilder& ToolbarBuilder, UObject* InContextObject);

	TSharedRef<FExtender> GetExtender(UObject* InContextObject);

	const TSharedRef<FUICommandList> CommandList;

	UObject* ContextObject;
};
