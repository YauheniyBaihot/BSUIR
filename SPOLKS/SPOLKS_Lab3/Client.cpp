#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

int StartClient(SOCKET *Listener, char* IpAddress, char* Port, char* Log);
char *GetFileName(SOCKET Listener);
int GetFileLength(SOCKET Listener, int *BytesCount);
int ReadCountOfSendedBytesFromLogFile();
void WriteCountOfSendedBytesToLogFile(int BytesCount);

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
	if(argc < 3)
	{
		return 0;
	}
	int BytesCount = 0; 
	char* Log = new char[100];
	FILE *file;
	SOCKET Listener;
	if(!StartClient(&Listener, argv[1], argv[2], Log))
	{
		printf("Connected to %s %s\n", argv[1], argv[2]);	
		char FileName[106];
		strcpy(FileName, GetFileName(Listener));
		int FileLength = GetFileLength(Listener, &BytesCount);
		while(true)
		{
			char buf[2];
			int Response = 0;
			fd_set fdread,fdOOB;
			BOOL isOOB = false;
			FD_ZERO(&fdread);
			FD_ZERO(&fdOOB);
			FD_SET(Listener, &fdread);
			FD_SET(Listener, &fdOOB);
			if(select(0, &fdread, 0, &fdOOB, 0) == SOCKET_ERROR)
			{
				printf("select() failed: %d\n",WSAGetLastError());
				return 0;
			}
			else
			{
				if(FD_ISSET(Listener,&fdOOB))
				{
					char bbb[1];
					if(recv(Listener, bbb, 1, MSG_OOB) > 0)
					{
						printf("Bytes Count: %d\n", BytesCount);
					}
					send(Listener, "ready", 6 * sizeof(char), 0);     
				} 
				else
				{
					file = fopen(FileName, "ab");
					Response = recv(Listener, buf, 2, 0);
					if (Response <= 0)
					{
						printf("Connection Closed\n");
						SocketHelper::CloseSocket(Listener);
						fclose(file);
						if(FileLength > BytesCount)
						{
							WriteCountOfSendedBytesToLogFile(BytesCount);					
						}
						break;
					}
					fwrite(buf, 1, Response, file);
					BytesCount += Response;
					send(Listener, "ready", 6 * sizeof(char), 0);
					fclose (file);
				}
			}
		}
	}
	getch();
	return 0;
}

int StartClient(SOCKET *Listener, char* IpAddress, char* Port, char* Log)
{
	sockaddr_in ListenerName;
	int Result = SocketHelper::InitializeSocket(Listener, &ListenerName, inet_addr(IpAddress), htons(atoi(Port)), Log);
	if(!Result)
	{
		int Answer = connect(*Listener, (const sockaddr*)&ListenerName, sizeof(ListenerName));
		if (Answer != 0)
		{
			strcpy(Log, "Failed to connect socket\n");
			return 0;
		}
	}
	return Result;
}

char *GetFileName(SOCKET Listener)
{
	char FileNameBufer[106];
	recv(Listener, FileNameBufer, 106, 0);
	string FileName(FileNameBufer);
	FileNameBufer[FileName.find_last_of('#', 105)] = '\0';
	send(Listener, "ready", 6 * sizeof(char), 0);
	return FileNameBufer;
}

int GetFileLength(SOCKET Listener, int *BytesCount)
{
	char FileLengthBufer[15];
	recv(Listener, FileLengthBufer, 15, 0);
	*BytesCount = ReadCountOfSendedBytesFromLogFile();
	if(*BytesCount != 0)
	{
		char *bufer = new char[15];
		itoa(*BytesCount, bufer, 10);
		send(Listener, bufer, strlen(bufer) * sizeof(char), 0);
	}
	else
	{
		send(Listener, "ready", 6 * sizeof(char), 0);
	}
	return atoi(FileLengthBufer);
}

int ReadCountOfSendedBytesFromLogFile()
{
	ifstream File("log.txt");
	string Temp;
	getline(File, Temp);
	File.close();
	return atoi(Temp.c_str());
}

void WriteCountOfSendedBytesToLogFile(int BytesCount)
{
	ofstream File("log.txt", ios::trunc);
	File << BytesCount;	
	File.close();
}