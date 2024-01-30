#pragma once

#include "Framework/Commands/Commands.h"

class FMEditorCommands : public TCommands<FMEditorCommands>
{
	
public:
	
	FMEditorCommands()
		: TCommands<FMEditorCommands>(
			TEXT("MEditor"),
			NSLOCTEXT("Contexts", "MCommon", "MEditor Editor"),
			NAME_None,
			"MEditorStyle")
	{
	}

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> StartGameService;
	TSharedPtr<FUICommandInfo> StopGameService;
	TSharedPtr<FUICommandInfo> ShowGddInFileManager;
	TSharedPtr<FUICommandInfo> ShowExcelInFileManager;
	//TSharedPtr<FUICommandInfo> UpdateGdd;
	//TSharedPtr<FUICommandInfo> ReloadGdd;
};
