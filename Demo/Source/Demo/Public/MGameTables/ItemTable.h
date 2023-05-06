#pragma once
#include "Engine/DataTable.h"

#include "MGameTypes.h"

// =========================================================
// *** BEGIN WRITING YOUR CODE - INCLUDE ***

// *** END WRITING YOUR CODE - INCLUDE ***
// =========================================================
#include "ItemTable.generated.h"


// =============================================================================
// *** BEGIN WRITING YOUR CODE - CUSTOMIZE ***

// *** END WRITING YOUR CODE - CUSTOMIZE ***
// =============================================================================


USTRUCT()
struct DEMO_API FItemRow : public FTableRowBase
{
    GENERATED_BODY()

    /** 道具Id */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSS")
    int32 Id;

    /** 道具名称 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSS")
    FString Name;

    /** 分类 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSS")
    int32 Type;

    /** 品质 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSS")
    int32 Quality;

    /** 叠加上限 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSS")
    int32 MaxNum;

    /** 售价 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSS")
    int32 SellingPrice;

    /** 道具描述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSS")
    FString Description;

    /** 道具图标 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectSS")
    TSoftObjectPtr<UTexture2D> Icon;

};

UCLASS(BlueprintType)
class DEMO_API UItemConfig : public UObject
{
    GENERATED_BODY()
public: 

    bool Init();

    /** 道具Id */
    UPROPERTY(BlueprintReadOnly, Category = "ProjectSS")
    int32 Id;

    /** 道具名称 */
    UPROPERTY(BlueprintReadOnly, Category = "ProjectSS")
    FString Name;

    /** 分类 */
    UPROPERTY(BlueprintReadOnly, Category = "ProjectSS")
    int32 Type;

    /** 品质 */
    UPROPERTY(BlueprintReadOnly, Category = "ProjectSS")
    int32 Quality;

    /** 叠加上限 */
    UPROPERTY(BlueprintReadOnly, Category = "ProjectSS")
    int32 MaxNum;

    /** 售价 */
    UPROPERTY(BlueprintReadOnly, Category = "ProjectSS")
    int32 SellingPrice;

    /** 道具描述 */
    UPROPERTY(BlueprintReadOnly, Category = "ProjectSS")
    FString Description;

    /** 道具图标 */
    UPROPERTY(BlueprintReadOnly, Category = "ProjectSS")
    TSoftObjectPtr<UTexture2D> Icon;

    // =========================================================
    // *** BEGIN WRITING YOUR CODE - UItemConfig ***

    // *** END WRITING YOUR CODE - UItemConfig ***
    // =========================================================
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnForeachItemConfig, const UItemConfig*, Entry);


UCLASS(BlueprintType)
class DEMO_API UItemConfigManager : public UObject
{
    GENERATED_BODY()
public:

    bool Init();
    void Foreach(const TFunction<bool(const UItemConfig*)>& Func);

    /** 查找 */
    UFUNCTION(BlueprintPure, Category = "ProjectSS") 
    const UItemConfig* Get(const int32& InKey) const; 

    /** 遍历 */
    UFUNCTION(BlueprintCallable, Category = "ProjectSS", DisplayName = "Foreach")
    void K2_Foreach(const FOnForeachItemConfig& InCallback); 

    /** 配置文件名称 */
    UFUNCTION(BlueprintPure, Category = "ProjectSS")
    FString GetConfigFileName() const;

    // =========================================================
    // *** BEGIN WRITING YOUR CODE - UItemConfigManager ***

    // *** END WRITING YOUR CODE - UItemConfigManager ***
    // =========================================================
    
private:

    UPROPERTY()
    TMap<int32, UItemConfig*> Data; 
    
};
