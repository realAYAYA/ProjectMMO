#include "MEditorCommands.h"

#define LOCTEXT_NAMESPACE "FMEditorCommands"

void FMEditorCommands::RegisterCommands()
{
	UI_COMMAND(StartGameService, "Start GameServer", "Start Game Service", EUserInterfaceActionType::Button,
			   FInputChord());
	UI_COMMAND(StopGameService, "Stop GameServer", "Stop Game Service", EUserInterfaceActionType::Button,
			   FInputChord());
	UI_COMMAND(ShowGddInFileManager, "打开 GDD 文件夹", "Show GDD In FileManager", EUserInterfaceActionType::Button,
			   FInputChord());
	UI_COMMAND(ShowExcelInFileManager, "打开 Excel 文件夹", "Show Excel In FileManager", EUserInterfaceActionType::Button,
			   FInputChord());
	/*UI_COMMAND(UpdateGdd, "Excel 打表", "Generate GDD data & Code", EUserInterfaceActionType::Button,
			   FInputChord());
	UI_COMMAND(ReloadGdd, "重新加载 GDD 表格", "Reload GDD tables", EUserInterfaceActionType::Button,
				  FInputChord());*/
}

#undef LOCTEXT_NAMESPACE
