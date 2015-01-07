
#pragma once

#if defined _WINDOWS_ || defined WIN32
	#define _WIN32_WINNT 0x0501
#endif // _DEBUG

#include "Socket.h"
#include "TcpSocketListener.h"
#include "TcpSocketServer.h"
#include "TcpSocketClient.h"
#include "UdpSocket.h"
#include "TLSPointer.h"

#include "Lock.h"
#include "StackQueue.h"

#include "BlockingQueue.h"
#include "CircleQueue.h"

#include "Log.h"

#include "MemoryPool.h"
#include "ObjectPool.h"

#include "ScopeGuard.h"

