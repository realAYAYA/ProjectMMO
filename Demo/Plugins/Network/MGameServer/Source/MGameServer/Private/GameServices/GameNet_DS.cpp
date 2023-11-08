#include "MGameSession.h"
#include "GameSessionHelper.h"

// 仅用于DS与WebSocket之间的通信，客户端禁止调用这些方法

M_GAME_RPC_HANDLE(GameRpc, LoginAsDS, InSession, Req, Ack)
{
	//FRole* Role = InSession->Role;
	//if (!Role)
	{
		return;
	}
}