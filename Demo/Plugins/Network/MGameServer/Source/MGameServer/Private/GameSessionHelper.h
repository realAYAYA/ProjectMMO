#pragma once

class FGameSession;

#ifndef MMODULENAME
#define MMODULENAME Default
#endif

#define M_GAME_MESSAGE_HANDLE_REGISTRANT_VARNAME PREPROCESSOR_JOIN(_MVAR_,PREPROCESSOR_JOIN(MMODULENAME,PREPROCESSOR_JOIN(_,__LINE__)))

struct FRpcServerHandleInitializers
{
	static FRpcServerHandleInitializers& Get();
	static void TearDown();

	void Register(const char* InRpcInterfaceName, const TFunction<void(FGameSession*, void* InRpcInterface)>& Func);
	void Bind(FGameSession* InGameSession, const FString& InRpcInterfaceName, void* InRpcInterfacePtr);

	template<typename T>
	void Bind(FGameSession* InGameSession, T* Ptr)
	{
		Bind(InGameSession, Ptr->GetName(), (void*)Ptr);
	}
	
private:

	TMap<FString, TArray<TFunction<void(FGameSession*, void*)>>> Handles;
};

#define M_GAME_RPC_HANDLE(RpcInterfaceName, RpcMethodName, GameSessionVar, ReqVar, AckVar) \
struct M_GAME_MESSAGE_HANDLE_REGISTRANT_TYPENAME \
{ \
	M_GAME_MESSAGE_HANDLE_REGISTRANT_TYPENAME() \
	{ \
		FRpcServerHandleInitializers::Get().Register(PREPROCESSOR_TO_STRING(RpcInterfaceName), [this](FGameSession* InSession, void* InRpcInterface) { \
			((PREPROCESSOR_JOIN(F,PREPROCESSOR_JOIN(RpcInterfaceName,Interface))*)InRpcInterface)->PREPROCESSOR_JOIN(On,RpcMethodName) = [this, InSession](const PREPROCESSOR_JOIN(F,PREPROCESSOR_JOIN(RpcMethodName,Req))& InReq, PREPROCESSOR_JOIN(F,PREPROCESSOR_JOIN(RpcMethodName,Ack)& InAck)) { \
				this->Handle(InSession, InReq, InAck); \
			}; \
		}); \
	} \
	void Handle(const FGameSession*, const PREPROCESSOR_JOIN(F,PREPROCESSOR_JOIN(RpcMethodName,Req))&, PREPROCESSOR_JOIN(F,PREPROCESSOR_JOIN(RpcMethodName,Ack))&); \
};	\
static M_GAME_MESSAGE_HANDLE_REGISTRANT_TYPENAME M_GAME_MESSAGE_HANDLE_REGISTRANT_VARNAME; \
void M_GAME_MESSAGE_HANDLE_REGISTRANT_TYPENAME::Handle(const FGameSession* GameSessionVar, const PREPROCESSOR_JOIN(F,PREPROCESSOR_JOIN(RpcMethodName,Req))& ReqVar, PREPROCESSOR_JOIN(F,PREPROCESSOR_JOIN(RpcMethodName,Ack)& AckVar))
