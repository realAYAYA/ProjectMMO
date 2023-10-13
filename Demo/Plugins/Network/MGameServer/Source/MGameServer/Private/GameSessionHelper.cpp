#include "GameSessionHelper.h"

#include "Misc/LazySingleton.h"

FRpcServerHandleInitializers& FRpcServerHandleInitializers::Get()
{
	return TLazySingleton<FRpcServerHandleInitializers>::Get();
}

void FRpcServerHandleInitializers::TearDown()
{
	
}

void FRpcServerHandleInitializers::Register(
	const char* InRpcInterfaceName,
	const TFunction<void(FGameSession*, void* InRpcInterface)>& Func)
{
	TArray<TFunction<void(FGameSession*, void*)>>* ContPtr = Handles.Find(InRpcInterfaceName);
	if (!ContPtr)
	{
		ContPtr = &Handles.Emplace(InRpcInterfaceName);
	}
    
	ContPtr->Emplace(Func);
}

void FRpcServerHandleInitializers::Bind(
	FGameSession* InGameSession,
	const FString& InRpcInterfaceName,
	void* InRpcInterfacePtr)
{
	TArray<TFunction<void(FGameSession*, void*)>>* ContPtr = Handles.Find(InRpcInterfaceName);
	if (!ContPtr)
		return;
	
	for (auto& Elem : *ContPtr)
	{
		Elem(InGameSession, InRpcInterfacePtr);
	}
}
