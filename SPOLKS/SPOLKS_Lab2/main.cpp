#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		return 0;
	}
	WSADATA WsaData;
	int WSAStartupReturnValue = WSAStartup( MAKEWORD( 2, 2 ), &WsaData );
	if ( WSAStartupReturnValue != 0 )
	{
		printf("Can't find usable wsock32.dll\n");
		return 0;
	}
	if ( LOBYTE( WsaData.wVersion ) != 2 || HIBYTE( WsaData.wVersion ) != 2 )
	{
		printf("Wsock32.dll of wrong version\n");
		return 0; 
	}
	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listener == 0)
	{
		printf("Failed to create socket\n");
		return 0;
	}
	sockaddr_in ListenerName;
	ListenerName.sin_family = AF_INET;
	ListenerName.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	ListenerName.sin_port = htons(atoi(argv[2]));
	int ret = bind(listener, (const sockaddr*)&ListenerName, sizeof(ListenerName));
	if (ret != 0)
	{
		printf("Failed to bind socket\n");
		return 0;
	}
	ret = listen(listener,SOMAXCONN);
	if (ret != 0)
	{
		printf("Failed to put socket into listening state\n");
		return 0;
	}
	while (1)
	{
		SOCKET *ClientSocket = (SOCKET*)malloc(sizeof(SOCKET));
		*ClientSocket = accept(listener, NULL, NULL);
		if (*ClientSocket != INVALID_SOCKET)
		{
			char buf[1024];
			int rlen;
			printf("Client thread (socket %d) is ready\n", *ClientSocket);
			while (1)
			{
				rlen = recv(*ClientSocket, buf, 1024, 0);
				printf("%d:", *ClientSocket);
				for(int i = 0; i < rlen; i++)
				{
					printf("%c", buf[i]);
				}
				printf("\n");
				if (rlen == 0)
				{
					printf("Socket %d closed the connection\n", *ClientSocket);
					closesocket(*ClientSocket);
					break;
				}
				else 
				{
					if (rlen == SOCKET_ERROR)
					{
						printf("Recv socket error at socket %d\n", *ClientSocket);
						closesocket(*ClientSocket);
						break;
					}
					send(*ClientSocket, buf, rlen, 0);
				}
			}
		}
		else
		{
			printf("Failed to accept the connection\n");
		}
	}
	return 0;
}