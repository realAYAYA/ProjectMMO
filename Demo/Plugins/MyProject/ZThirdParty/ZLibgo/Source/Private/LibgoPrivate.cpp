#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


#define LOCTEXT_NAMESPACE "FZLibgoModule"

class FZLibgoModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
	}
	
	virtual void ShutdownModule() override
	{
	}
};

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FZLibgoModule, ZLibgo)
