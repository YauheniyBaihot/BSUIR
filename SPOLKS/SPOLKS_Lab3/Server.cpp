#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

int StartServer(SOCKET *ClientSocket, char* IpAddress, char* Port, char* Log);
void SendFileName(char *FilePath, SOCKET ClientSocket);
int SendFileLength(FILE *file, SOCKET ClientSocket);
bool KeyPressed(int key);

class SocketHelper
{

public:
	static __declspec(dllexport) int InitializeSocket(SOCKET *Listener, sockaddr_in *ListenerName, ULONG IPAddress, USHORT Port, char* Log);
	static __declspec(dllexport) void CloseSocket(SOCKET Socket);
};

int SocketHelper::InitializeSocket(SOCKET *Socket, sockaddr_in *SocketAddress, ULONG IPAddress, USHORT Port, char* Log)
{
	WSADATA WsaData;
	int WSAStartupReturnValue = WSAStartup( MAKEWORD( 2, 2 ), &WsaData );
	if ( WSAStartupReturnValue != 0 )
	{
		strcpy(Log, "Can't find usable wsock32.dll\n");
		return 1;
	}
	if ( LOBYTE( WsaData.wVersion ) != 2 || HIBYTE( WsaData.wVersion ) != 2 )
	{
		strcpy(Log, "Wsock32.dll of wrong version\n");
		return 2; 
	}
	*Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*Socket == 0)
	{
		strcpy(Log, "Failed to create socket\n");
		return 3;
	}
	SocketAddress->sin_family = AF_INET;
	SocketAddress->sin_addr.S_un.S_addr = IPAddress;
	SocketAddress->sin_port = Port;
	return 0;
}

void SocketHelper::CloseSocket(SOCKET Socket)
{
	shutdown(Socket, 2);
	WSACleanup();
}

int main(int argc, char** argv)
{
	if(argc < 4)
	{
		return 0;
	}
	while(true)
	{
		int symbols;	
		char* Log = new char[100];
		SOCKET ClientSocket;	
		printf("Server start\n");
		StartServer(&ClientSocket, argv[1], argv[2], Log);
		char *FilePath = argv[3];
		FILE *file;
		file = fopen(FilePath,"r");
		if(file != NULL)
		{			
			SendFileName(FilePath, ClientSocket);			
			fseek(file, SendFileLength(file, ClientSocket), SEEK_SET);
			printf("Start sending\n");
			while(!feof(file)) 
			{				
				char buf[1024];
				char bufer[2];
				if(KeyPressed(VK_UP))
				{
					int result = send(ClientSocket, "~", 1, MSG_OOB);
					printf("Send out of band data\n");
				}	
				else
				{
					symbols = fread(bufer, 1, 2, file);
					send(ClientSocket, bufer, symbols, 0);
				}
				if(recv(ClientSocket, buf, sizeof(buf), 0) <= 0)
				{					
					break;
				}							
			}	
			fclose(file);
			SocketHelper::CloseSocket(ClientSocket);
			printf("Connection Closed\n");
		}		
	}
	getch();
	return 0;
}

int StartServer(SOCKET *ClientSocket, char* IpAddress, char* Port, char* Log)
{
	SOCKET Listener;
	sockaddr_in ListenerName;
	int Result = SocketHelper::InitializeSocket(&Listener, &ListenerName, inet_addr(IpAddress), htons(atoi(Port)), Log);
	if(!Result)
	{
		int Answer = bind(Listener, (const sockaddr*)&ListenerName, sizeof(ListenerName));
		if (Answer != 0)
		{
			strcpy(Log, "Failed to bind socket\n");
			return 0;
		}
		Answer = listen(Listener,SOMAXCONN);
		if (Answer != 0)
		{
			strcpy(Log, "Failed to put socket into listening state\n");
			return 0;
		}
		*ClientSocket = accept(Listener, NULL, NULL);
	}
	return Result;
}

void SendFileName(char *FilePath, SOCKET ClientSocket)
{
	char buf[1024];
	char *FileName = new char[100];
	char *Extension = new char[5];
	_splitpath(FilePath, new char[1], new char[200], FileName, Extension);
	strcat(FileName, Extension);
	strcat(FileName, "#");
	send(ClientSocket, FileName, strlen(FileName) + 1, 0);
	recv(ClientSocket, buf, sizeof(buf), 0);
}

int SendFileLength(FILE *file, SOCKET ClientSocket)
{
	char buf[1024];
	char *FileLength = new char[15];
	itoa(filelength(fileno(file)), FileLength, 10);		
	send(ClientSocket, FileLength, strlen(FileLength), 0);
	recv(ClientSocket, buf, sizeof(buf), 0);
	return atoi(buf);
}

bool KeyPressed(int key)
{
	return (GetAsyncKeyState(key) & 0x8000 != 0);
}