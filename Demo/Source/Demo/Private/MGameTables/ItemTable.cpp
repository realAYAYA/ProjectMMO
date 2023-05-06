#include "MGameTables/ItemTable.h"
#include "GameTables/Public/ConfigLoadHelper.h"


// =============================================================================
// *** BEGIN WRITING YOUR CODE - CUSTOMIZE ***

// *** END WRITING YOUR CODE - CUSTOMIZE ***
// =============================================================================


bool UItemConfig::Init()
{
    // =========================================================
    // *** BEGIN WRITING YOUR CODE - UItemConfig::Init ***

    // *** END WRITING YOUR CODE - UItemConfig::Init ***
    // =========================================================
    return true;
} 

bool UItemConfigManager::Init()
{
    FString Path = GetGameDesignDataFullPath() / GetConfigFileName();
    auto Table = LoadTableFromJsonFile<FItemRow>(Path, TEXT("Id"));
    if (!Table)
        return false;
    
    Table->ForeachRow<FItemRow>(
        TEXT("UItemConfigManager::Init"), 
        [this](const FName& Key, const FItemRow& Row)
        {
            bool bIsNew = true;
            UItemConfig* Config = nullptr;
            {
                auto Ret = Data.Find(Row.Id);
                if (Ret && *Ret)
                {
                    bIsNew = false;
                    Config = *Ret;
                    for (TFieldIterator<FProperty> It(UItemConfig::StaticClass()); It; ++It)
                    {
                        FProperty* Prop = *It;
                        if (!Prop)
                            continue;
                        void* ValuePtr = Prop->ContainerPtrToValuePtr<void>(Config, 0);
                        if (!ValuePtr)
                            continue;
                        Prop->InitializeValue(ValuePtr);
                    }
                }
                else
                {
                    Config = NewObject<UItemConfig>();
                }
            }
            Config->Id = Row.Id;
            Config->Name = Row.Name;
            Config->Type = Row.Type;
            Config->Quality = Row.Quality;
            Config->MaxNum = Row.MaxNum;
            Config->SellingPrice = Row.SellingPrice;
            Config->Description = Row.Description;
            Config->Icon = Row.Icon;
            if (Config->Init())
            {
                if (bIsNew)
                {
                    Data.Emplace(Config->Id, Config);
                }
            }
        });

    // =========================================================
    // *** BEGIN WRITING YOUR CODE - UItemConfigManager::Init ***

    // *** END WRITING YOUR CODE - UItemConfigManager::Init ***
    // =========================================================
    return true;
}

void UItemConfigManager::Foreach(const TFunction<bool(const UItemConfig*)>& Func)
{
    for (const auto& Elem : Data)
    {
        if (!Func(Elem.Value)) 
            break;
    }
}
 
const UItemConfig* UItemConfigManager::Get(const int32& InKey) const 
{
    auto Ret = Data.Find(InKey);
    if (!Ret)
        return nullptr;
    return *Ret;
}

void UItemConfigManager::K2_Foreach(const FOnForeachItemConfig& InCallback)
{
    Foreach([InCallback](const UItemConfig* Entry) -> bool {
        InCallback.Execute(Entry);
        return true;
    });
}

FString UItemConfigManager::GetConfigFileName() const
{
    return TEXT("Item.jsondata");
}
