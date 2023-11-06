#include "CommonPrivate.h"

#define LOCTEXT_NAMESPACE "FMGameCommonModule"


/**
 * Implements the ZCommonModule module and the network messaging extension modular feature.
 */
class FMGameCommonModule
	: public FSelfRegisteringExec
	, public IModuleInterface
{
	
public:

	//~ FSelfRegisteringExec interface

	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override
	{
		return false;
	}

public:

	// IModuleInterface interface
	virtual void StartupModule() override
	{

	}

	virtual void ShutdownModule() override
	{

	}

};

IMPLEMENT_MODULE(FMGameCommonModule, ZCommon);

#undef LOCTEXT_NAMESPACE
