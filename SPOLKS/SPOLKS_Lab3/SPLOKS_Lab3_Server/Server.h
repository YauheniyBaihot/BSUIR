#pragma once

#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

class Server
{
	SOCKET Listener;		
	sockaddr_in ListenerName;

public:
	SOCKET ClientSocket;

	Server(void);
	~Server(void);

	int StartServer(char* Log);
	void CloseServer();
	int Recv(char *Buffer, int Symbols);
	void Send(char *Buffer, int Symbols);
};

