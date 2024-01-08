#include "Net/MNetFwd.h"


uint64 FRpcManager::AddRpcCallback(const FOnRpcResult& InCallback)
{
	if (RequestPendingData.Find(SerialNum + 1))
	{
		InCallback.Execute(EOpErrorCode::Waiting);
		return 0;
	}
	
	SerialNum += 1;
	RequestPendingData.Add(SerialNum, FRpcPendingData(InCallback));
	return SerialNum;
}

const FOnRpcResult* FRpcManager::FindRpcCallback(const uint64 In)
{
	const FRpcPendingData* PendingData = RequestPendingData.Find(In);
	if (PendingData)
	{
		if (IsValid(PendingData->Callback.GetUObject()))
		{
			if (!PendingData->IsTimeOut())
				return &PendingData->Callback;
			else
			{
				PendingData->Callback.Execute(EOpErrorCode::Timeout);
			}
		}

		RequestPendingData.Remove(In);
	}

	return nullptr;
}

void FRpcManager::RemoveRpcCallback(const uint64 In)
{
	RequestPendingData.Remove(In);
}

const FOnRpcResult* FRpcManager::FindRpcCallback(const FString& In)
{
	const FRpcPendingData* PendingData = RequestPendingDataByString.Find(In);
	if (PendingData)
	{
		if (IsValid(PendingData->Callback.GetUObject()))
		{
			if (!PendingData->IsTimeOut())
				return &PendingData->Callback;
			else
			{
				PendingData->Callback.Execute(EOpErrorCode::Timeout);
			}
		}

		RequestPendingDataByString.Remove(In);
	}

	return nullptr;
}

void FRpcManager::AddRpcCallback(const FString& In, const FOnRpcResult& InCallback)
{
	if (RequestPendingDataByString.Find(In))
	{
		InCallback.Execute(EOpErrorCode::Waiting);
		return;
	}
	
	RequestPendingDataByString.Add(In, FRpcPendingData(InCallback));
}

void FRpcManager::RemoveRpcCallback(const FString& In)
{
	RequestPendingDataByString.Remove(In);
}
