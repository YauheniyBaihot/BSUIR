#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

void RunTCPServer(char** argv);
void RunUDPServer(char** argv);
int StartTCPServer(SOCKET *ClientSocket, char* IpAddress, char* Port, char* Log);
int StartUDPServer(SOCKET *ClientSocket, char* IpAddress, char* Port, char* Log);
char *GetFileNameByTCP(SOCKET Listener);
char *GetFileNameByUDP(SOCKET Listener, sockaddr_in* Client, int* Client_Length);
void GetFileLengthAndStartPositionByTCP(SOCKET Listener, int *FileLength, int *BytesCount);
void GetFileLengthAndStartPositionByUDP(SOCKET Listener, int *FileLength, int *BytesCount, sockaddr_in* Client, int* Client_Length);

#ifndef SocketHelper

class SocketHelper
{

public:
	static __declspec(dllexport) int InitializeSocket(SOCKET *Listener, sockaddr_in *ListenerName, int SocketType, ULONG IPAddress, USHORT Port, char* Log);
	static __declspec(dllexport) void CloseSocket(SOCKET Socket);
	static __declspec(dllexport) void RunProcesses(int argc, char** argv, LPWSTR ProcessPath);
	static __declspec(dllexport) void RunCreateProcessFunction(int argc, char** argv, char* type, LPWSTR ProcessPath);
	static __declspec(dllexport) string CmdArgumentsToLine(int argc, char **argv, char *type);
	static __declspec(dllexport) void string_to_wstring(const std::string& src, std::wstring& dest);
};

int SocketHelper::InitializeSocket(SOCKET *Socket, sockaddr_in *SocketAddress, int SocketType, ULONG IPAddress, USHORT Port, char* Log)
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
	*Socket = socket(AF_INET, SocketType, 0);
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

void SocketHelper::RunProcesses(int argc, char** argv, LPWSTR ProcessPath)
{
	RunCreateProcessFunction(argc, argv, "TCP", ProcessPath);
	RunCreateProcessFunction(argc, argv, "UDP", ProcessPath);
}

void SocketHelper::RunCreateProcessFunction(int argc, char** argv, char* type, LPWSTR ProcessPath)
{
	STARTUPINFO SI;
	ZeroMemory(&SI, sizeof(STARTUPINFO));
	PROCESS_INFORMATION PI;
	string Src = SocketHelper::CmdArgumentsToLine(argc, argv, type);
	wstring Dest;
	string_to_wstring(Src, Dest);
	CreateProcess(ProcessPath, (LPWSTR)Dest.c_str(), NULL, NULL, FALSE, NULL, NULL, NULL, &SI, &PI);
}

void SocketHelper::string_to_wstring(const std::string& src, std::wstring& dest)
{
	std::wstring tmp;
	tmp.resize(src.size());
	std::transform(src.begin(),src.end(),tmp.begin(),btowc);
	tmp.swap(dest);
}

string SocketHelper::CmdArgumentsToLine(int argc, char **argv, char *type)
{
	string Temp;
	for(int i = 0; i < argc; i++)
	{		
		Temp.append(argv[i]);
		Temp.append(" ");
	}	
	Temp.append(type);
	return Temp;
}

#endif

int main(int argc, char** argv)
{	
	/*if(argc == 3)
	{
		SocketHelper::RunProcesses(argc, argv, L"..\\Debug\\Server.exe");
	}
	else
	{
		if(argc == 4)
		{
			printf("%s Server start\n", argv[3]);
			if(!strcmp(argv[3], "TCP"))
			{
				RunTCPServer(argv);
			}
			else
			{*/
				if(!strcmp(argv[3], "UDP"))
				{					
					RunUDPServer(argv);
				}
			/*}
			getch();
		}
	}*/
	return 0;
}

void RunTCPServer(char** argv)
{	
	while(true)
	{		
		char* Log = new char[100];
		SOCKET ClientSocket;			
		StartTCPServer(&ClientSocket, argv[1], argv[2], Log);
		char FileName[106];
		strcpy(FileName, GetFileNameByTCP(ClientSocket));
		int BytesCount = 0, FileLength = 0;
		GetFileLengthAndStartPositionByTCP(ClientSocket, &FileLength, &BytesCount);
		while(true)
		{
			FILE *file;
			char bufer[1000000];
			int Response = 0;
			fd_set fdread,fdOOB;
			BOOL isOOB = false;
			FD_ZERO(&fdread);
			FD_ZERO(&fdOOB);
			FD_SET(ClientSocket, &fdread);
			FD_SET(ClientSocket, &fdOOB);
			if(select(0, &fdread, 0, &fdOOB, 0) == SOCKET_ERROR)
			{
				printf("select() failed: %d\n",WSAGetLastError());
				return;
			}
			else
			{
				if(FD_ISSET(ClientSocket, &fdOOB))
				{
					char bbb[1];
					if(recv(ClientSocket, bbb, 1, MSG_OOB) > 0)
					{
						printf("Bytes Count: %d\n", BytesCount);
					}
					send(ClientSocket, "ready", 6 * sizeof(char), 0);     
				} 
				else
				{
					file = fopen(FileName, "ab");
					Response = recv(ClientSocket, bufer, 1000000, 0);
					if (Response <= 0)
					{
						printf("Connection Closed\n");
						SocketHelper::CloseSocket(ClientSocket);
						fclose(file);
						break;
					}
					fwrite(bufer, 1, Response, file);
					BytesCount += Response;
					send(ClientSocket, "ready", 6 * sizeof(char), 0);
					fclose (file);
				}
			}
		}
	}
}

void RunUDPServer(char** argv)
{
	while(true)
	{		
		struct sockaddr_in Client;
		int Client_Length = (int)sizeof(struct sockaddr_in);
		char* Log = new char[100];
		SOCKET ClientSocket;				
		StartUDPServer(&ClientSocket, argv[1], argv[2], Log);
		char FileName[106];
		strcpy(FileName, GetFileNameByUDP(ClientSocket, &Client, &Client_Length));
		int BytesCount = 0, FileLength = 0;
		GetFileLengthAndStartPositionByUDP(ClientSocket, &FileLength, &BytesCount, &Client, &Client_Length);
		while(true)
		{
			FILE *file;
			char bufer[1000000];
			int Response = 0;
			
			file = fopen(FileName, "ab");
			Response = recvfrom(ClientSocket, bufer, 1000000, 0, (struct sockaddr *)&Client, &Client_Length);
			if (Response <= 0)
			{
				printf("Connection Closed\n");
				SocketHelper::CloseSocket(ClientSocket);
				fclose(file);
				break;
			}
			fwrite(bufer, 1, Response, file);
			BytesCount += Response;
			sendto(ClientSocket, "ready", 6 * sizeof(char), 0, (struct sockaddr *)&Client, Client_Length);
			fclose (file);
		}
	}
}

int StartTCPServer(SOCKET *ClientSocket, char* IpAddress, char* Port, char* Log)
{
	SOCKET Listener;
	sockaddr_in ListenerName;
	int Result = SocketHelper::InitializeSocket(&Listener, &ListenerName, SOCK_STREAM, inet_addr(IpAddress), htons(atoi(Port)), Log);
	if(!Result)
	{
		int Answer = bind(Listener, (const sockaddr*)&ListenerName, sizeof(ListenerName));		
		if (Answer != 0)
		{
			strcpy(Log, "Failed to bind socket\n");
			return 0;
		}
		Answer = listen(Listener, SOMAXCONN);
		if (Answer != 0)
		{
			strcpy(Log, "Failed to put socket into listening state\n");
			return 0;
		}
		*ClientSocket = accept(Listener, NULL, NULL);
	}
	return Result;
}

int StartUDPServer(SOCKET *ClientSocket, char* IpAddress, char* Port, char* Log)
{
	SOCKET Listener;
	sockaddr_in ListenerName;	
	int Result = SocketHelper::InitializeSocket(&Listener, &ListenerName, SOCK_DGRAM, inet_addr(IpAddress), htons(atoi(Port)), Log);	
	if(!Result)
	{
		int Answer = bind(Listener, (const sockaddr*)&ListenerName, sizeof(ListenerName));	
		if (Answer != 0)
		{
			strcpy(Log, "Failed to bind socket\n");
			return 0;
		}			
		*ClientSocket = Listener;		
	}			
	return Result;
}

char *GetFileNameByTCP(SOCKET Listener)
{
	char FileNameBufer[106];
	recv(Listener, FileNameBufer, 106, 0);
	string FileName(FileNameBufer);
	FileNameBufer[FileName.find_last_of('#', 105)] = '\0';
	send(Listener, "ready", 6 * sizeof(char), 0);
	return FileNameBufer;
}

char *GetFileNameByUDP(SOCKET Listener, sockaddr_in* Client, int* Client_Length)
{
	char FileNameBufer[106];
	recvfrom(Listener, FileNameBufer, 106, 0, (struct sockaddr *)Client, Client_Length);
	string FileName(FileNameBufer);
	FileNameBufer[FileName.find_last_of('#', 105)] = '\0';
	sendto(Listener, "ready", 6 * sizeof(char), 0, (struct sockaddr *)Client, *Client_Length);
	return FileNameBufer;
}

void GetFileLengthAndStartPositionByTCP(SOCKET Listener, int *FileLength, int *BytesCount)
{
	char *bufer = new char[15];
	recv(Listener, bufer, 15, 0);
	*FileLength = atoi(bufer);
	send(Listener, "ready", 6 * sizeof(char), 0);
	bufer = new char[15];
	recv(Listener, bufer, 15, 0);
	*BytesCount = atoi(bufer);
	send(Listener, "ready", 6 * sizeof(char), 0);	
}

void GetFileLengthAndStartPositionByUDP(SOCKET Listener, int *FileLength, int *BytesCount, sockaddr_in* Client, int* Client_Length)
{
	char *bufer = new char[15];
	recvfrom(Listener, bufer, 15, 0, (struct sockaddr *)Client, Client_Length);
	*FileLength = atoi(bufer);
	sendto(Listener, "ready", 6 * sizeof(char), 0, (struct sockaddr *)Client, *Client_Length);
	bufer = new char[15];
	recvfrom(Listener, bufer, 15, 0, (struct sockaddr *)Client, Client_Length);
	*BytesCount = atoi(bufer);
	sendto(Listener, "ready", 6 * sizeof(char), 0, (struct sockaddr *)Client, *Client_Length);	
}