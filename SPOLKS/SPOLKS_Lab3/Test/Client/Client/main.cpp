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
void SendFileName(char *FilePath, SOCKET ClientSocket);
void SendFileLengthAndStartPosition(FILE *file, SOCKET Socket, int *FileLength, int *BytesCount);
bool KeyPressed(int key);
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
	char* Log = new char[100];
	FILE *file;
	SOCKET Listener;
	if(!StartClient(&Listener, argv[1], argv[2], Log))
	{
		printf("Connected to %s %s\n", argv[1], argv[2]);	
		int symbols;	
		int BytesCount = 0, FileLength = 0;
		char *FilePath = argv[3];
		FILE *file;
		file = fopen(FilePath,"rb");
		if(file != NULL)
		{
			SendFileName(FilePath, Listener);
			SendFileLengthAndStartPosition(file, Listener, &FileLength, &BytesCount);
			if(BytesCount)
			{
				fseek(file, BytesCount, SEEK_SET);
			}
			printf("Start sending\n");
			while(BytesCount != FileLength) 
			{				
				char buf[6];
				char bufer[1000];
				if(KeyPressed(VK_UP))
				{
					int result = send(Listener, "~", 1, MSG_OOB);
					printf("Send out of band data\n");
					if(recv(Listener, buf, sizeof(buf), 0) <= 0)
					{					
						break;
					}	
				}	
				else
				{
					symbols = fread(bufer, 1, 1000, file);
					send(Listener, bufer, symbols, 0);
					if(recv(Listener, buf, sizeof(buf), 0) <= 0)
					{					
						break;
					}	
					BytesCount += symbols;
				}								
			}	
			fclose(file);
			SocketHelper::CloseSocket(Listener);
			printf("Connection Closed\n");
			if(FileLength > BytesCount)
			{
				WriteCountOfSendedBytesToLogFile(BytesCount);					
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

void SendFileName(char *FilePath, SOCKET Socket)
{
	char buf[1024];
	char *FileName = new char[100];
	char *Extension = new char[5];
	_splitpath(FilePath, new char[1], new char[200], FileName, Extension);
	strcat(FileName, Extension);
	strcat(FileName, "#");
	send(Socket, FileName, strlen(FileName) + 1, 0);
	recv(Socket, buf, sizeof(buf), 0);
}

void SendFileLengthAndStartPosition(FILE *file, SOCKET Socket, int *FileLength, int *BytesCount)
{
	char buf[1024];
	char *bufer = new char[15];
	*FileLength = filelength(fileno(file));
	itoa(*FileLength, bufer, 10);		
	send(Socket, bufer, strlen(bufer), 0);
	recv(Socket, buf, sizeof(buf), 0);
	*BytesCount = ReadCountOfSendedBytesFromLogFile();
	bufer = new char[15];
	itoa(*BytesCount, bufer, 10);
	send(Socket, bufer, strlen(bufer) * sizeof(char), 0);
	recv(Socket, buf, sizeof(buf), 0);
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

bool KeyPressed(int key)
{
	return (GetAsyncKeyState(key) & 0x8000 != 0);
}