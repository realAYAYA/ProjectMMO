#pragma once

#include "GameMessage.h"
#include "NetFwd.h"

class MRPC_API FMessageDispatcher
{
	
public:

	template <typename MessageType>
	void Reg(const TFunction<void(const FConnectionPtr&, const MessageType&)>& InCallback)
	{
		uint64 Code = MessageType::kTypeId;

		// 注册消息原型
		{
			MessageType Dummy;
			RegMessageType(Code, MessageType::StaticStruct());
		}
		
		Callbacks.Emplace(Code, [InCallback](const FConnectionPtr& InConn, const MessageType& InMessage)
		{
			auto Message = StaticCastSharedPtr<MessageType>(InMessage);
			if (Message)
			{
				InCallback(InConn, Message);
			}
		});
	}

	void Process(const FConnectionPtr& InConn, uint64 InCode, const char* InDataPtr, int32 InDataLen);
	void Process(const FConnectionPtr& InConn, uint64 InCode, const FGameMessage& InMessage);

	void SetUnknownMessageCallback(const TFunction<void(const FGameMessage&)>& Func);

private:
	
	TMap<uint64, TFunction<void(const FConnectionPtr&, const FGameMessage&)>> Callbacks;
	TFunction<void(const FGameMessage&)> UnknownMessageCallback;

	static void RegMessageType(const uint64 InTypeID, const UScriptStruct* InType);
	static TMap<uint64, const UScriptStruct*> MessageTypes;  // 消息Code和原型对应表
};
