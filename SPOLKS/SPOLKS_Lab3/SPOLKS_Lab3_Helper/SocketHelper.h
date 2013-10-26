#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

class SocketHelper
{

public:
	static __declspec(dllexport) int InitializeSocket(SOCKET *Listener, sockaddr_in *ListenerName, ULONG IPAddress, USHORT Port, char* Log);
	static __declspec(dllexport) void CloseSocket(SOCKET Socket);
	static __declspec(dllexport) int Recv(SOCKET Socket, char *Bufer, int Symbols);
	static __declspec(dllexport) void Send(SOCKET Socket, char *Bufer, int Symbols);
};

