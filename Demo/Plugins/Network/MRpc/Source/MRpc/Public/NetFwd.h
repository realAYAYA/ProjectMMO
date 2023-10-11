#pragma once

class IWebSocket;
typedef TSharedPtr<IWebSocket, ESPMode::ThreadSafe> FConnectionPtr;
typedef TWeakPtr<IWebSocket, ESPMode::ThreadSafe> FConnectionWeakPtr;
