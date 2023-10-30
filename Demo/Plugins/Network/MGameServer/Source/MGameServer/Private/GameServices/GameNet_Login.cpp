#include "MGameServerSubsystem.h"

#include "..\MGameSession.h"
#include "GameSessionHelper.h"

M_GAME_RPC_HANDLE(GameRpc, LoginGame, InSession, Req, Ack)
{
	//FRole* Role = InSession->Role;
	//if (!Role)
	{
		Ack.Ret = ELoginGameRetCode::DuplicateLogin;
		Ack.bReLogin = true;
		return;
	}
}
