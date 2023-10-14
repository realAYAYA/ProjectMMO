// Copyright Epic Games, Inc. All Rights Reserved.

#include "MGameServerBPLibrary.h"
#include "MGameServerPrivate.h"

UMGameServerBPLibrary::UMGameServerBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

FString UMGameServerBPLibrary::NetMessageToString(const FLoginGameReq& Param)
{
	FNetworkMessage RpcMessage;
	RpcMessage.TypeID = FLoginGameReq::KeyTypeID;
	RpcMessage.RpcErrorCode = ERpcErrorCode::Unimplemented;
	RpcMessage.RpcMessageOp = ERpcMessageOp::Request;
	RpcMessage.SerialNum = 1;
	Param.SerializeToArray(RpcMessage.SetBody());

	TArray<uint8> Data;
	RpcMessage.SerializeToArray(Data);
	
	const std::string cstr(reinterpret_cast<const char*>(Data.GetData()), Data.Num());
	FString Str = UTF8_TO_TCHAR(cstr.c_str());
	
	return Str;
}

FLoginGameReq UMGameServerBPLibrary::StringToNetMessage(const FString& Param)
{
	TArray<uint8> Data;
	Data.Append((uint8 *)TCHAR_TO_ANSI(*Param), Param.Len());

	FNetworkMessage RpcMessage;
	RpcMessage.ParseFromArray(Data);
	
	FLoginGameReq LoginGameReq;
	LoginGameReq.ParseFromArray(RpcMessage.GetBody());
	return LoginGameReq;
}

FLoginGameReq UMGameServerBPLibrary::SerializeTest(const FLoginGameReq& Param)
{
	FNetworkMessage RpcMessage;
	RpcMessage.TypeID = FLoginGameReq::KeyTypeID;
	RpcMessage.RpcErrorCode = ERpcErrorCode::Unimplemented;
	RpcMessage.RpcMessageOp = ERpcMessageOp::Request;
	RpcMessage.SerialNum = 1;
	Param.SerializeToArray(RpcMessage.SetBody());

	TArray<uint8> Data;
	RpcMessage.SerializeToArray(Data);

	FNetworkMessage RpcMessage2;
	RpcMessage2.ParseFromArray(Data);
	
	FLoginGameReq LoginGameReq;
	LoginGameReq.ParseFromArray(RpcMessage2.GetBody());
	return LoginGameReq;
}

