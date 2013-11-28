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

void RunClient(char** argv, int SocketType);
int StartClient(SOCKET *Listener, int SocketType, char* IpAddress, char* Port, char* Log);
void SendFileName(char *FilePath, SOCKET ClientSocket);
void SendFileLengthAndStartPosition(FILE *file, SOCKET Socket, int *FileLength, int *BytesCount);
void GetStartPosition(FILE *file, SOCKET Socket, int *FileLength, int *BytesCount);
int ReadCountOfSendedBytesFromLogFile();
void WriteCountOfSendedBytesToLogFile(int BytesCount);
bool KeyPressed(int key);

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
	if(argc == 4)
	{
		SocketHelper::RunProcesses(argc, argv, L"..\\Debug\\Client.exe");
	}
	else
	{
		if(argc == 5)
		{
			printf("%s Client start\n", argv[4]);
			if(!strcmp(argv[4], "TCP"))
			{
				RunClient(argv, SOCK_STREAM);
				return 0;
			}
			else
			{
				if(!strcmp(argv[4], "UDP"))
				{
					RunClient(argv, SOCK_DGRAM);
				}
			}
			getch();
		}
	}
	return 0;
}

void RunClient(char** argv, int SocketType)
{
	char* Log = new char[100];
	FILE *file;
	SOCKET Listener;
	if(!StartClient(&Listener, SocketType, argv[1], argv[2], Log))
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
			GetStartPosition(file, Listener, &FileLength, &BytesCount);
			//SendFileLengthAndStartPosition(file, Listener, &FileLength, &BytesCount);
			if(BytesCount)
			{
				fseek(file, BytesCount, SEEK_SET);
			}
			printf("Start sending\n");
			while(BytesCount != FileLength) 
			{                                
				char buf[6];
				char bufer[65536];
				if(KeyPressed(VK_UP) && SocketType == SOCK_STREAM)
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
					symbols = fread(bufer, 1, 65536, file);
					send(Listener, bufer, symbols, 0);
					if(recv(Listener, buf, sizeof(buf), 0) <= 0)
					{                                        
						break;
					}        
					BytesCount += symbols;
				}                                                                
			}        
			Sleep(1000);
			fclose(file);
			SocketHelper::CloseSocket(Listener);
			printf("Connection Closed\n");
			if(FileLength > BytesCount)
			{
				WriteCountOfSendedBytesToLogFile(BytesCount);                                        
			}
			else
			{
				WriteCountOfSendedBytesToLogFile(0);
			}
		}                
	}
}

int StartClient(SOCKET *Listener, int SocketType, char* IpAddress, char* Port, char* Log)
{
	sockaddr_in ListenerName;
	int Result = SocketHelper::InitializeSocket(Listener, &ListenerName, SocketType, inet_addr(IpAddress), htons(atoi(Port)), Log);
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

void GetStartPosition(FILE *file, SOCKET Socket, int *FileLength, int *BytesCount)
{
	char *bufer = new char[15];
	*FileLength = filelength(fileno(file));
	itoa(*FileLength, bufer, 10);                
	send(Socket, bufer, strlen(bufer), 0);
	recv(Socket, bufer, 15, 0);
	*BytesCount = atoi(bufer);
}

bool KeyPressed(int key)
{
	return (GetAsyncKeyState(key) & 0x8000 != 0);
}