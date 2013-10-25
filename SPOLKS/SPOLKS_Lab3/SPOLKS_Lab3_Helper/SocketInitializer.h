#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

class SocketInitializer
{
public:
	static __declspec(dllexport) int InitializeSocket(SOCKET *Listener, sockaddr_in *ListenerName, ULONG IPAddress, USHORT Port, char* Log);
};

