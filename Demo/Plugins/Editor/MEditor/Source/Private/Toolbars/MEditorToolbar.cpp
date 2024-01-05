#include "MEditorToolbar.h"
//#include "EditorCommands.h"
#include "MEditorCommands.h"
#include "MGameServerPrivate.h"
#include "Animation/AnimInstance.h"
#include "Layout/Children.h"

#define LOCTEXT_NAMESPACE "FMToolbarModule"

FMGameServerModule* GetMGameServerModule()
{
	EModuleLoadResult LoadResult;
	if (auto Mod = FModuleManager::Get().LoadModuleWithFailureReason(TEXT("MGameServer"), LoadResult))
	{
		return StaticCast<FMGameServerModule*>(Mod);
	}
	
	return nullptr;
}

// ============================================================================

FMEditorToolbar::FMEditorToolbar()
	: CommandList(new FUICommandList)
{
	ContextObject = nullptr;
}

void FMEditorToolbar::Initialize()
{
	BindCommands();
}

void FMEditorToolbar::StartGameService_Executed()
{
	if (const auto Mod = GetMGameServerModule())
	{
		Mod->Start();
	}
}

void FMEditorToolbar::StopGameService_Executed()
{
	if (const auto Mod = GetMGameServerModule())
	{
		Mod->Shutdown();
	}
}

bool GeneratePerforceConfig()
{
	FString SCCProvider;
	FString P4Port;
	FString P4User;
	FString P4Client;

	bool bOk = false;
	do
	{
		FString IniFile;
		{
			// 配置项参见 Engine/Source/Developer/SourceControl/Private/SourceControlHelpers.cpp
			const FString SourceControlSettingsDir = FPaths::GeneratedConfigDir();
			FConfigCacheIni::LoadGlobalIniFile(IniFile, TEXT("SourceControlSettings"), nullptr, false, false, true,
			                                   true, *SourceControlSettingsDir);
		}
		
		{
			// 配置项参见 Engine/Source/Developer/SourceControl/Private/SourceControlSettings.cpp
			static const FString SettingsSection = TEXT("SourceControl.SourceControlSettings");
			GConfig->GetString(*SettingsSection, TEXT("Provider"), SCCProvider, IniFile);
			if (SCCProvider != TEXT("Perforce"))
				break;
		}
		
		{
			// 配置项参见
			// Engine/Plugins/Developer/PerforceSourceControl/Source/PerforceSourceControl/Private/PerforceSourceControlSettings.cpp
			static const FString SettingsSection = TEXT("PerforceSourceControl.PerforceSourceControlSettings");
			if (!GConfig->GetString(*SettingsSection, TEXT("Port"), P4Port, IniFile))
				break;
			if (!GConfig->GetString(*SettingsSection, TEXT("UserName"), P4User, IniFile))
				break;
			if (!GConfig->GetString(*SettingsSection, TEXT("Workspace"), P4Client, IniFile))
				break;
		}
		
		bOk = true;
	}
	while (false);
	
	if (!bOk)
	{
		const FString Text = FString::Printf(TEXT("未正确配置 Perforce"));
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Text));
		return false;
	}

	const FString P4ConfigFile = FPaths::EngineDir() + "../.p4config"; // 在工作区根目录
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (IFileHandle* FileHandle = PlatformFile.OpenWrite(*P4ConfigFile))
	{
		const auto Body = FString::Printf(
			TEXT("P4PORT=%s\nP4USER=%s\nP4CLIENT=%s\nP4CHARSET=utf8\n"), *P4Port, *P4User, *P4Client);
		FileHandle->Write((const uint8*)TCHAR_TO_ANSI(*Body), Body.Len());

		delete FileHandle;
	}
	else
	{
		const FString Text = FString::Printf(TEXT("创建P4配置文件失败! %s"), *P4ConfigFile);
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Text));
		
		return false;
	}
	
	return true;
}

bool ExecuteShellScript(const FString& ScriptFile, const FString& WorkDir)
{
#if PLATFORM_WINDOWS
	FString ScriptExt = TEXT(".bat");
	FString CmdExe = TEXT("cmd.exe");
#elif PLATFORM_LINUX
	FString ScriptExt = TEXT(".sh");
	FString CmdExe = TEXT("/bin/bash");
#else
	FString ScriptExt = TEXT(".sh");
	FString CmdExe = TEXT("/bin/sh");
#endif

	int32 ReturnCode = 0;
	const FString Params(ScriptFile + ScriptExt);
	FProcHandle RunningProc = FPlatformProcess::CreateProc(*CmdExe, *Params, true, false, false, nullptr, 0,
	                                                       *WorkDir, nullptr);
	FPlatformProcess::WaitForProc(RunningProc);
	FPlatformProcess::GetProcReturnCode(RunningProc, &ReturnCode);
	FPlatformProcess::CloseProc(RunningProc);
	UE_LOG(LogTemp, Display, TEXT("Code=%d"), ReturnCode);

	return ReturnCode == 0;
}

bool ExecutePyScript(const FString& Params, const FString& WorkDir)
{
	// FString PlatformName = FPlatformProperties::PlatformName();
	const FString BinDirName = FPlatformProcess::GetBinariesSubdirectory();
	FString PyBin = FPaths::EngineDir() + TEXT("Binaries/ThirdParty/Python3/") + BinDirName;
	if (BinDirName == TEXT("Win64"))
	{
		PyBin += TEXT("/python");
	}
	else
	{
		PyBin += TEXT("/bin/python");
	}

	PyBin = FPaths::ConvertRelativePathToFull(PyBin);

	int32 ReturnCode = 0;
	FProcHandle RunningProc = FPlatformProcess::CreateProc(
		*PyBin,
		*Params,
		true,
		false,
		false,
		nullptr,
		0,
		*WorkDir,
		nullptr);
	
	FPlatformProcess::WaitForProc(RunningProc);
	FPlatformProcess::GetProcReturnCode(RunningProc, &ReturnCode);
	FPlatformProcess::CloseProc(RunningProc);

	return ReturnCode == 0;
}

void FMEditorToolbar::ShowGddInFileManager_Executed()
{
	const FString Dir = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() + TEXT("GDD"));
	FPlatformProcess::ExploreFolder(*Dir);
}

void FMEditorToolbar::ShowExcelInFileManager_Executed()
{
	const FString Dir = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() + TEXT("../GameDesignData/Excel"));
	FPlatformProcess::ExploreFolder(*Dir);
}

/*
void FMEditorToolbar::UpdateGdd_Executed()
{
	if (!GeneratePerforceConfig()) // 重新生成P4配置文件
		return;

	{
		FString Text = TEXT("你确定要打表吗？");
		if (FMessageDialog::Open(EAppMsgType::YesNo, FText::FromString(Text)) != EAppReturnType::Yes)
			return;
	}

	FString WorkDir = FPaths::ConvertRelativePathToFull(FPaths::EnginePluginsDir() + TEXT("ZGameTables/"));
	FString LogFile = FPaths::ConvertRelativePathToFull(FPaths::ProjectIntermediateDir() + TEXT("make_gdd_err.log"));
	FString Params = WorkDir + "make.py -log_file=" + LogFile;
	bool bOk = ExecutePyScript(Params, WorkDir);

	FString Text;
	if (bOk)
	{
		Text = TEXT("成功");
	}
	else
	{
		Text = TEXT("失败!");

#if PLATFORM_WINDOWS
		{
			FProcHandle Proc = FPlatformProcess::CreateProc(
				TEXT("c:\\Windows\\notepad.exe"), *LogFile, true, false, false, NULL, 0,
				*WorkDir, NULL);
			if (!Proc.IsValid())
			{
				FPlatformProcess::CloseProc(Proc);
			}
		}
#endif		
	}
	
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Text));

	if (bOk)
	{
		auto GameTables = FZGameTablesModule::Get().GetGameTables();
		GameTables->Init();
	}
}

void FMEditorToolbar::ReloadGdd_Executed()
{
	auto GameTables = FZGameTablesModule::Get().GetGameTables();
	GameTables->Init();

	{
		FString Text = TEXT("完成");
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Text));
	}
}
*/

void FMEditorToolbar::BindCommands()
{
	const auto& Commands = FMEditorCommands::Get();
	CommandList->MapAction(Commands.StartGameService, FExecuteAction::CreateRaw(this, &FMEditorToolbar::StartGameService_Executed));
	CommandList->MapAction(Commands.StopGameService, FExecuteAction::CreateRaw(this, &FMEditorToolbar::StopGameService_Executed));
	CommandList->MapAction(Commands.ShowGddInFileManager, FExecuteAction::CreateRaw(this, &FMEditorToolbar::ShowGddInFileManager_Executed));
	CommandList->MapAction(Commands.ShowExcelInFileManager, FExecuteAction::CreateRaw(this, &FMEditorToolbar::ShowExcelInFileManager_Executed));
	//CommandList->MapAction(Commands.UpdateGdd, FExecuteAction::CreateRaw(this, &FMEditorToolbar::UpdateGdd_Executed));
	//CommandList->MapAction(Commands.ReloadGdd, FExecuteAction::CreateRaw(this, &FMEditorToolbar::ReloadGdd_Executed));
}

TSharedRef<SWidget> FMEditorToolbar::GenerateProjectMMenuContent(TSharedRef<FUICommandList> InCommandList)
{
	const FMEditorCommands& Commands = FMEditorCommands::Get();
	FMenuBuilder MenuBuilder(true, InCommandList);

	bool bGameServiceRunning = false;
	{
		const FMGameServerModule* Mod = GetMGameServerModule();
		if (Mod && Mod->IsRunning())
		{
			bGameServiceRunning = true;
		}
	}

	if (!bGameServiceRunning)
	{
		MenuBuilder.AddMenuEntry(Commands.StartGameService);
	}
	else
	{
		MenuBuilder.AddMenuEntry(Commands.StopGameService);
	}
			
	MenuBuilder.AddSeparator();
	MenuBuilder.AddMenuEntry(Commands.ShowGddInFileManager);
	MenuBuilder.AddMenuEntry(Commands.ShowExcelInFileManager);
	//MenuBuilder.AddMenuEntry(Commands.UpdateGdd);
	//MenuBuilder.AddMenuEntry(Commands.ReloadGdd);
			
	return MenuBuilder.MakeWidget();
}

void FMEditorToolbar::BuildToolbar(FToolBarBuilder& ToolbarBuilder, UObject* InContextObject)
{
	// InContextObject 在关卡模式为 nullptr， 在蓝图模式下则为当前蓝图对象

	ToolbarBuilder.BeginSection(NAME_None);

	ToolbarBuilder.AddComboButton(
		FUIAction(),
		FOnGetContent::CreateLambda([&, InContextObject]()
		{
			ContextObject = InContextObject;
			
			return GenerateProjectMMenuContent(CommandList);
		}),
		LOCTEXT("MEditor_Label", "ProjectM Editor"),
		LOCTEXT("MEditor_ToolTip", "ProjectM Editor"),
		FSlateIcon("MEditorStyle", TEXT("MEditor.Icon1"))
	);

	ToolbarBuilder.EndSection();
}

TSharedRef<FExtender> FMEditorToolbar::GetExtender(UObject* InContextObject)
{
	const auto ExtensionDelegate = FToolBarExtensionDelegate::CreateLambda(
		[this, InContextObject](FToolBarBuilder& ToolbarBuilder)
		{
			BuildToolbar(ToolbarBuilder, InContextObject);
		});
	
	TSharedRef<FExtender> ToolbarExtender(new FExtender());
	if (InContextObject)
		ToolbarExtender->AddToolBarExtension("Debugging", EExtensionHook::After, CommandList, ExtensionDelegate);
	else
		ToolbarExtender->AddToolBarExtension("Compile", EExtensionHook::After, CommandList, ExtensionDelegate);
	
	return ToolbarExtender;
}

#undef LOCTEXT_NAMESPACE
