#pragma once

class MRPC_API FMessageDispatcher
{
public:

	/*template <typename PbType>
	void Reg(const TFunction<void(const FZPbConnectionPtr&, const TSharedPtr<PbType>&)>& InCallback)
	{
		uint64 Code = PbType::kTypeId;

		// 注册消息原型
		{
			PbType Dummy;
			RegMessagePrototype(Code, Dummy.GetDescriptor());
		}
		
		Callbacks.Emplace(Code, [InCallback](const FZPbConnectionPtr& InConn, const TSharedPtr<FPbMessage>& InMessage)
		{
			auto Message = StaticCastSharedPtr<PbType>(InMessage);
			if (Message)
			{
				InCallback(InConn, Message);
			}
		});
	}

	void Process(const FZPbConnectionPtr& InConn, uint64 InCode, const char* InDataPtr, int32 InDataLen);
	void Process(const FZPbConnectionPtr& InConn, uint64 InCode, const TSharedPtr<FPbMessage>& InMessage);

	void SetUnknownMessageCallback(const TFunction<void(const TSharedPtr<FPbMessage>&)>& Func);

	static TSharedPtr<FPbMessage> ParsePbFromArray(uint64 PbCode, const char* DataPtr, uint32 DataLength);
	static TSharedPtr<FPbMessage> ParsePbFromArray(uint64 PbCode, const FZDataBufferPtr& InBuffer);
	
private:
	TMap<uint64, TFunction<void(const FZPbConnectionPtr&, const TSharedPtr<FPbMessage>&)>> Callbacks;
	TFunction<void(const TSharedPtr<FPbMessage>&)> UnknownMessageCallback;
	
	static void RegMessagePrototype(uint64 InPbCode, const google::protobuf::Descriptor* InPbDescriptor);
	static TMap<uint64, const google::protobuf::Message*> MessagePrototypes;  // 消息Code和原型对应表
	*/
	
};