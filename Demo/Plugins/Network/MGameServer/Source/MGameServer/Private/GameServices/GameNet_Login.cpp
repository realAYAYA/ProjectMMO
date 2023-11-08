#include "MGameSession.h"
#include "GameSessionHelper.h"

M_GAME_RPC_HANDLE(GameRpc, LoginGame, InSession, Req, Ack)
{
	UMPlayer* Player = InSession->Player;
	if (!Player)
	{
		Ack.Ret = ELoginGameRetCode::DuplicateLogin;
		Ack.bReLogin = true;
		return;
	}
}
