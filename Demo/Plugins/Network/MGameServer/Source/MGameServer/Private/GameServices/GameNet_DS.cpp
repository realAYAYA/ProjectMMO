#include "MGameSession.h"
#include "GameSessionHelper.h"

// 仅用于DS与WebSocket之间的通信，客户端禁止调用这些方法

M_GAME_RPC_HANDLE(GameRpc, LoginAsDS, InSession, Req, Ack)
{
	// Todo 服务器拉起DS进程
	// 1. 服务器开始，设置关卡信息，存储在UWorld
	// 2. 创建进程，以命令行形式将关卡启动，包含Token（验证用，7位数随机码），指定端口号
	// 3. DS进程启动，并在关卡初始化完成后，根据给定Token和端口号，发送该Rpc请求
	// 4. 服务器收到后将进程Handle保管好
	// 5. 如果有需要，比如这是私人地下城，则还会记录该地下城的持有玩家信息，并返回给创建者
	// 6. 玩家如何进入地下城或大世界？正常的公共区域大世界有着固定ID，玩家请求进入即可，也可以指定地下城ID进入（当然会有一系列许可检定）
	
	UMWorld* World = InSession->World;
	if (!World)
	{
		return;
	}
	
}