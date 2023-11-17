#include "MTools.h"

#include <regex>

#include "Misc/Fnv.h"

void FStringToString(const FString& In, std::string* Out)
{
	*Out = TCHAR_TO_UTF8(*In);
}

void StringToFString(const std::string& In, FString* Out)
{
	*Out = UTF8_TO_TCHAR(In.c_str());
}

FString ToChineseNumbers(int32 In)
{
	FString Out;
	switch (In) // TODO: 先简单罗列，后续再优化
	{
	case 0: Out = TEXT("零"); break;
	case 1: Out = TEXT("一"); break;
	case 2: Out = TEXT("二"); break;
	case 3: Out = TEXT("三"); break;
	case 4: Out = TEXT("四"); break;
	case 5: Out = TEXT("五"); break;
	case 6: Out = TEXT("六"); break;
	case 7: Out = TEXT("七"); break;
	case 8: Out = TEXT("八"); break;
	case 9: Out = TEXT("九"); break;
	case 10: Out = TEXT("十"); break;
	case 11: Out = TEXT("十一"); break;
	case 12: Out = TEXT("十二"); break;
	case 13: Out = TEXT("十三"); break;
	case 14: Out = TEXT("十四"); break;
	case 15: Out = TEXT("十五"); break;
	case 16: Out = TEXT("十六"); break;
	case 17: Out = TEXT("十七"); break;
	case 18: Out = TEXT("十八"); break;
	case 19: Out = TEXT("十九"); break;
	case 20: Out = TEXT("二十"); break;
	case 21: Out = TEXT("二十一"); break;
	case 22: Out = TEXT("二十二"); break;
	case 23: Out = TEXT("二十三"); break;
	case 24: Out = TEXT("二十四"); break;
	case 25: Out = TEXT("二十五"); break;
	case 26: Out = TEXT("二十六"); break;
	case 27: Out = TEXT("二十七"); break;
	case 28: Out = TEXT("二十八"); break;
	case 29: Out = TEXT("二十九"); break;
	case 30: Out = TEXT("三十"); break;
	case 31: Out = TEXT("三十一"); break;
	case 32: Out = TEXT("三十二"); break;
	case 33: Out = TEXT("三十三"); break;
	case 34: Out = TEXT("三十四"); break;
	case 35: Out = TEXT("三十五"); break;
	case 36: Out = TEXT("三十六"); break;
	case 37: Out = TEXT("三十七"); break;
	case 38: Out = TEXT("三十八"); break;
	case 39: Out = TEXT("三十九"); break;
	case 40: Out = TEXT("四十"); break;
	case 41: Out = TEXT("四十一"); break;
	case 42: Out = TEXT("四十二"); break;
	case 43: Out = TEXT("四十三"); break;
	case 44: Out = TEXT("四十四"); break;
	case 45: Out = TEXT("四十五"); break;
	case 46: Out = TEXT("四十六"); break;
	case 47: Out = TEXT("四十七"); break;
	case 48: Out = TEXT("四十八"); break;
	case 49: Out = TEXT("四十九"); break;
	case 50: Out = TEXT("五十"); break;		
	default: Out = LexToString(In); break;
	}

	return Out;
}

bool IsChinese(const FString& In)
{
	static std::regex Pattern("^[\u4e00-\u9fa5]+$");

	std::string Text;
	FStringToString(In, &Text);
	
	return std::regex_match(Text, Pattern);
}

uint64 GenerateUID(const FString& In)
{
	return FFnv::MemFnv64(*In, In.Len());
}

// ============================================================================

// TODO: GetProjectVersion 需要补上 IOS 和 PC 的版本号获取方式，现在只有 Android 是对的

#if PLATFORM_ANDROID
FString GetProjectVersion()  // Android
{
	FConfigFile AndroidEngineSettings;
	FConfigCacheIni::LoadLocalIniFile(AndroidEngineSettings, TEXT("Engine"), true, TEXT("Android"));

	FString Version;
	AndroidEngineSettings.GetString(TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"), TEXT("VersionDisplayName"), Version);
	
	return Version;		
}
#elif PLATFORM_IOS
FString GetProjectVersion()  // IOS, 现在是临时处理，直接读取 Android 配置
{
	FConfigFile AndroidEngineSettings;
	FConfigCacheIni::LoadLocalIniFile(AndroidEngineSettings, TEXT("Engine"), true, TEXT("Android"));

	FString Version;
	AndroidEngineSettings.GetString(TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"), TEXT("VersionDisplayName"), Version);
	
	return Version;
}
#else
FString GetProjectVersion()  // PC
{
	return TEXT("0.0.1.0");
}
#endif

bool IsPureAlphabetString(const FString& InStr)
{
	for (int32 Idx = 0; Idx < InStr.Len(); ++Idx)
	{
		const TCHAR C = InStr[Idx];
		if (C >= TEXT('A') && C <= TEXT('Z'))
			continue;
		if (C >= TEXT('a') && C <= TEXT('z'))
			continue;
		if (C >= TEXT('0') && C <= TEXT('9'))
			continue;
		if (C == TEXT('_')) 
			continue;
		
		return false;
	}
	
	return true;
}

bool CheckTimeInDayRange(const FDateTime Now, const FTimespan Begin, const FTimespan End)
{
	bool bBeginOk = false;
	if (Now.GetHour() > Begin.GetHours())
	{
		bBeginOk = true;
	}
	else if (Now.GetHour() == Begin.GetHours())
	{
		if (Now.GetMinute() > Begin.GetMinutes())
		{
			bBeginOk = true;
		}
		else if (Now.GetMinute() == Begin.GetMinutes())
		{
			if (Now.GetSecond() >= Begin.GetSeconds())
				bBeginOk = true;
		}
	}

	bool bEndOk = false;
	if (Now.GetHour() < End.GetHours())
	{
		bEndOk = true;
	}
	else if (Now.GetHour() == End.GetHours())
	{
		if (Now.GetMinute() < End.GetMinutes())
		{
			bEndOk = true;
		}
		else if (Now.GetMinute() == End.GetMinutes())
		{
			if (Now.GetSecond() <= End.GetSeconds())
				bEndOk = true;
		}
	}	
	
	return bBeginOk && bEndOk;
}