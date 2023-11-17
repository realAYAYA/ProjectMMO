#pragma once

void MGAMECOMMON_API FStringToString(const FString& In, std::string* Out);
void MGAMECOMMON_API StringToFString(const std::string& In, FString* Out);

FString MGAMECOMMON_API ToChineseNumbers(int32 In);

bool MGAMECOMMON_API IsChinese(const FString& In);  // 是否为纯中文字符串

uint64 MGAMECOMMON_API GenerateUID(const FString& In);

/**
 * 百分之X的概率
 */
inline bool RollByPercent(float Percent)
{
	const int32 Roll = FMath::RandRange(0, 99);
	return Roll < Percent ? true : false;
}

/**
 * 千分之X的概率
 */
inline bool RollByThousand(float Thousand)
{
	const int32 Roll = FMath::RandRange(0, 999);
	return Roll < Thousand ? true : false;
}

/**
 * 万分之X的概率
 */
inline bool RollByTenThousand(float TenThousand)
{
	const int32 Roll = FMath::RandRange(0, 9999);
	return Roll < TenThousand ? true : false;
}

/**
 * 打乱数组内容
 */
template <typename T>
void ArrayShuffle(T& InArray)
{
	const int32 LastIndex = InArray.Num() - 1;
	for (int32 i = 0; i <= LastIndex; ++i)
	{
		if (int32 Index = FMath::RandRange(i, LastIndex); i != Index)
		{
			InArray.Swap(i, Index);
		}
	}	
}

/** 获取项目版本号 */ 
FString MGAMECOMMON_API GetProjectVersion();

/** 字符串是否只由字母、数字和下划线组成 */
bool MGAMECOMMON_API IsPureAlphabetString(const FString& InStr);

/** 检查时间是否在一天内的指定区间内 */
bool MGAMECOMMON_API CheckTimeInDayRange(const FDateTime Now, const FTimespan Begin, const FTimespan End);

/** 给定一组权重，随机roll，返回一个下标 */
//int32 RandomInArray(const TArray<int32>& Weights);