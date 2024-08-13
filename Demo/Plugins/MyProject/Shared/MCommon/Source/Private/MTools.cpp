#include "MTools.h"

FDateTime FMyTools::Now()
{
	return FDateTime::UtcNow();
}

FDateTime FMyTools::LocalNow()
{
	return FDateTime::Now();
}

// TODO(hudawei): ZGetProjectVersion 需要补上 IOS 和 PC 的版本号获取方式，现在只有 Android 是对的
#if PLATFORM_ANDROID
FString FMyTools::GetProjectVersion()  // Android
{
	FConfigFile AndroidEngineSettings;
	FConfigCacheIni::LoadLocalIniFile(AndroidEngineSettings, TEXT("Engine"), true, TEXT("Android"));

	FString Version;
	AndroidEngineSettings.GetString(TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"), TEXT("VersionDisplayName"), Version);
	
	return Version;		
}
#elif PLATFORM_IOS
FString FMyTools::GetProjectVersion()  // IOS, 现在是临时处理，直接读取 Android 配置
{
	FConfigFile AndroidEngineSettings;
	FConfigCacheIni::LoadLocalIniFile(AndroidEngineSettings, TEXT("Engine"), true, TEXT("Android"));

	FString Version;
	AndroidEngineSettings.GetString(TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"), TEXT("VersionDisplayName"), Version);
	
	return Version;		
}
#else
FString FMyTools::GetProjectVersion()
{
	return TEXT("0.0.1.0");	
}
#endif
