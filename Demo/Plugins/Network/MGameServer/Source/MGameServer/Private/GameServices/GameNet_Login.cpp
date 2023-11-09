#include "MGameSession.h"
#include "GameSessionHelper.h"
#include "MGameServerPrivate.h"
#include "MTools.h"
#include "RedisOp.h"
#include "Misc/Fnv.h"

M_GAME_RPC_HANDLE(GameRpc, LoginGame, InSession, Req, Ack)
{
	// Todo 检查版本
	
	UMPlayer* Player = InSession->Player;

	Ack.Ret = ELoginGameRetCode::UnKnow;
	
	// 重复请求登录
	if (Player)
	{
		return;
	}

	const int32 AccountLen = Req.Account.Len();  
	if (AccountLen < 1 || AccountLen > 20)
	{
		UE_LOG(LogMGameServer, Warning, TEXT("LoginGame失败, 用户名 %s 长度非法"), *Req.Account);
		return;
	}

	uint64 PlayerID = 0;
	if (!IsPureAlphabetString(Req.Account))
	{
		UE_LOG(LogMGameServer, Warning, TEXT("LoginGame失败, 用户名 %s 含有非法字符"), *Req.Account);
		return;
	}

	if (!FRedisOp::GetAccountInfo(Req.Account, &PlayerID))
	{
		UE_LOG(LogMGameServer, Warning, TEXT("LoginGame失败, 获取ID失败 Account = %s"), *Req.Account)
		return;
	}
	if (PlayerID == 0)
	{
		PlayerID = FFnv::MemFnv64(*Req.Account, Req.Account.Len());
	}

	
}
