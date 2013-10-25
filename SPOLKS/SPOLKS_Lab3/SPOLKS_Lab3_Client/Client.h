#pragma once

#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

class Client
{
	sockaddr_in ListenerName;

public:
	SOCKET Listener;		

	Client(void);
	~Client(void);

	int StartClient(char* Log);
	void CloseClient();
	int Recv(char *Buffer, int Symbols);
	void Send(char *Buffer, int Symbols);
};

