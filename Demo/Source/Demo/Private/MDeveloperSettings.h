#pragma once

#include "CoreMinimal.h"
#include "MDeveloperSettings.generated.h"

// Config=Game：储存在 DefaultGame.ini, 在Project-Settings中的Game分段
// default config： 存储在默认的配置文件里，而不是 项目目录-Saved-Config里面的那些
UCLASS(Config=Game,defaultconfig,meta=(DisplayName = "ProjectMMO Settings"))
class DEMO_API UMDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	// Config ： 将变量暴露至到上面UCLASS后指定的 ini配置文件里
	// EditAnywhere ：将变量暴露至 项目设置（Edit-ProjectSettings）窗口
	// AdvancedDisplay： 显示路径选择器
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General", AdvancedDisplay)
	TSoftObjectPtr<UDataTable> KeyboardMapping;

};