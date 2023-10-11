#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogGameNetwork, Log, All);

class IWebSocket;
typedef TSharedPtr<IWebSocket, ESPMode::ThreadSafe> FConnectionPtr;
typedef TWeakPtr<IWebSocket, ESPMode::ThreadSafe> FConnectionWeakPtr;

class INetworkingWebSocket;
typedef TSharedPtr<INetworkingWebSocket, ESPMode::ThreadSafe> FServerPtr;
typedef TWeakPtr<INetworkingWebSocket, ESPMode::ThreadSafe> FServerWeakPtr;
