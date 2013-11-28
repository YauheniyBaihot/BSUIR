<<<<<<< HEAD:SPOLKS/SPOLKS_Lab3/Test/Server/Server/main.cpp
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <time.h>
using namespace std;


void RunTCPServer(char** argv);
void RunUDPServer(char** argv);
int StartTCPServer(SOCKET *ClientSocket, SOCKET *Listener, char* IpAddress, char* Port, char* Log);
int StartUDPServer(SOCKET *ClientSocket, char* IpAddress, char* Port, char* Log);
char *GetFileNameByTCP(SOCKET Listener);
char *GetFileNameByUDP(SOCKET Listener, sockaddr_in* Client, int* Client_Length);
void GetFileLengthAndStartPositionByTCP(SOCKET Listener, int *FileLength, int *BytesCount);
void GetFileLengthAndStartPositionByUDP(SOCKET Listener, int *FileLength, int *BytesCount, sockaddr_in* Client, int* Client_Length);
void GetFileLengthAndStartPositionByTCP(SOCKET Listener, int *FileLength, int *BytesCount, char* FileName);
void GetFileLengthAndStartPositionByUDP(SOCKET Listener, int *FileLength, int *BytesCount, char* FileName, sockaddr_in* Client, int* Client_Length);
string ConvertIntToString(int Number);

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
	if(argc == 3)
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
			{
				if(!strcmp(argv[3], "UDP"))
				{                                        
					RunUDPServer(argv);
				}
			}
			getch();
		}
	}
	return 0;
}

void RunTCPServer(char** argv)
{        
	while(true)
	{                
		char* Log = new char[100];
		SOCKET ClientSocket, Listener;     		
		StartTCPServer(&ClientSocket, &Listener, argv[1], argv[2], Log);

		//
		int Number = 0;
		FD_SET ReadSet;
		int ReadySock;
		map <int,string> clients;
		clients.clear();
		typedef map<int,string>::value_type valType;
		//

		/*char FileName[106];
		strcpy(FileName, GetFileNameByTCP(ClientSocket));
		int BytesCount = 0, FileLength = 0;
		GetFileLengthAndStartPositionByTCP(ClientSocket, &FileLength, &BytesCount);*/
		while(true)
		{
			FILE *file;
			char bufer[1000];
			int Response = 0;

			//
			// Заполняем множество сокетов
			FD_ZERO(&ReadSet);
			FD_SET(Listener, &ReadSet);
			for(map<int,string>::iterator it = clients.begin(); it != clients.end(); it++)
				FD_SET((*it).first , &ReadSet);
			timeval timeout;
			timeout.tv_sec = 15;
			timeout.tv_usec = 0;
			int mx = Listener;
			for(map<int,string>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				if ((*it).first > Listener)
					mx = (*it).first;
			};
			// Ждём события в одном из сокетов
			if ((ReadySock = select(mx+1, &ReadSet, NULL, NULL, &timeout)) == SOCKET_ERROR)
			{
				printf("Select Error\n");
				return;
			}
			if (FD_ISSET(Listener, &ReadSet))
			{
				ClientSocket = accept(Listener, NULL, NULL);
				if (ClientSocket == INVALID_SOCKET)
				{
					return;
				}
				ULONG ulBlock = 1;
				if (ioctlsocket(ClientSocket, FIONBIO, &ulBlock) == SOCKET_ERROR)
				{
					return;
				}
				Sleep(1);
				char FileName[106];
				strcpy(FileName, GetFileNameByTCP(ClientSocket));
				int BytesCount = 0, FileLength = 0;
				GetFileLengthAndStartPositionByTCP(ClientSocket, &FileLength, &BytesCount, FileName);
				Number++;   // увеличиваем счетчик подключившихся клиентов
				string filename;//(ConvertIntToString(Number));
				filename.append(FileName);
				clients.insert( valType(ClientSocket,filename));
			}
			for(map<int,string>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				if(FD_ISSET((*it).first, &ReadSet))
				{
					FILE *F;
					if (!(F = fopen((*it).second.c_str() ,"ab")))
					{
						perror("Create File");
						break;
					};

					Response = recv((*it).first, bufer, 1000, 0);
					if(Response <= 0)
					{
						// Соединение разорвано, удаляем сокет из множества
						closesocket((*it).first);
						clients.erase((*it).first);						
						if(clients.size() == 0)
						{
							break;
						}
						else
						{
							it = clients.begin();
							continue;
						}
					}
					//записали данные в файл
					fwrite(bufer, sizeof(char), Response, F);
					// Отправляем данные обратно клиенту
					send((*it).first, "ready", 6 * sizeof(char), 0);

					fclose(F);
				}
				//printf("\nReceiving the part of file %d is complete.\n",Number);
			}    
			//

			/*file = fopen(FileName, "ab");
			Response = recv(ClientSocket, bufer, 1000, 0);
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
			fclose (file);*/
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
		SOCKET ClientSocket, Listener = 116;                                
		StartUDPServer(&ClientSocket, argv[1], argv[2], Log);

		//
		int Number = 0;
		FD_SET ReadSet;
		int ReadySock;
		map <int,string> clients;
		clients.clear();
		typedef map<int,string>::value_type valType;
		//

		/*char FileName[106];
		strcpy(FileName, GetFileNameByUDP(ClientSocket, &Client, &Client_Length));
		int BytesCount = 0, FileLength = 0;
		GetFileLengthAndStartPositionByUDP(ClientSocket, &FileLength, &BytesCount, &Client, &Client_Length);*/
		while(true)
		{
			FILE *file;
			char bufer[1000];
			int Response = 0;

			//
			// Заполняем множество сокетов
			FD_ZERO(&ReadSet);
			FD_SET(ClientSocket, &ReadSet);
			for(map<int,string>::iterator it = clients.begin(); it != clients.end(); it++)
				FD_SET((*it).first , &ReadSet);
			timeval timeout;
			timeout.tv_sec = 15;
			timeout.tv_usec = 0;
			int mx = ClientSocket;
			for(map<int,string>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				if ((*it).first > ClientSocket)
					mx = (*it).first;
			};
			// Ждём события в одном из сокетов
			if ((ReadySock = select(mx+1, &ReadSet, NULL, NULL, &timeout)) == SOCKET_ERROR)
			{
				printf("Select Error\n");
				return;
			}
			int x = FD_ISSET(ClientSocket, &ReadSet);
			if (FD_ISSET(Listener, &ReadSet))
			{
				ULONG ulBlock = 1;
				if (ioctlsocket(ClientSocket, FIONBIO, &ulBlock) == SOCKET_ERROR)
				{
					return;
				}
				Sleep(1);
				char FileName[106];
				strcpy(FileName, GetFileNameByUDP(ClientSocket, &Client, &Client_Length));
				int BytesCount = 0, FileLength = 0;
				GetFileLengthAndStartPositionByUDP(ClientSocket, &FileLength, &BytesCount, FileName, &Client, &Client_Length);
				Number++;   // увеличиваем счетчик подключившихся клиентов
				string filename;//(ConvertIntToString(Number));
				filename.append(FileName);
				clients.insert( valType(ClientSocket,filename));
				Listener = 13;
			}
			for(map<int,string>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				if(FD_ISSET((*it).first, &ReadSet))
				{
					FILE *F;
					if (!(F = fopen((*it).second.c_str() ,"ab")))
					{
						perror("Create File");
						break;
					};
					Response = recvfrom(ClientSocket, bufer, 1000, 0, (struct sockaddr *)&Client, &Client_Length);
					if(Response <= 0)
					{
						// Соединение разорвано, удаляем сокет из множества
						closesocket((*it).first);
						clients.erase((*it).first);						
						if(clients.size() == 0)
						{
							break;
						}
						else
						{
							it = clients.begin();
							continue;
						}
					}
					//записали данные в файл
					fwrite(bufer, sizeof(char), Response, F);
					// Отправляем данные обратно клиенту
					sendto(ClientSocket, "ready", 6 * sizeof(char), 0, (struct sockaddr *)&Client, Client_Length);

					fclose(F);
				}
				//printf("\nReceiving the part of file %d is complete.\n",Number);
			}    
			//

			/*file = fopen(FileName, "ab");
			Response = recvfrom(ClientSocket, bufer, 1000, 0, (struct sockaddr *)&Client, &Client_Length);
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
			fclose (file);*/
		}
	}
}

int StartTCPServer(SOCKET *ClientSocket, SOCKET *Listener, char* IpAddress, char* Port, char* Log)
{
	sockaddr_in ListenerName;
	int Result = SocketHelper::InitializeSocket(Listener, &ListenerName, SOCK_STREAM, inet_addr(IpAddress), htons(atoi(Port)), Log);
	if(!Result)
	{
		int Answer = bind(*Listener, (const sockaddr*)&ListenerName, sizeof(ListenerName));                
		if (Answer != 0)
		{
			strcpy(Log, "Failed to bind socket\n");
			return 0;
		}

		//
		ULONG ulBlock = 1;
		if (ioctlsocket(*Listener, FIONBIO, &ulBlock) == SOCKET_ERROR)
		{
			strcpy(Log, "Failed ioctlsocket\n");
			return 0;
		}
		//

		Answer = listen(*Listener, SOMAXCONN);
		if (Answer != 0)
		{
			strcpy(Log, "Failed to put socket into listening state\n");
			return 0;
		}
		//*ClientSocket = accept(Listener, NULL, NULL);
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

		//
		ULONG ulBlock = 1;
		if (ioctlsocket(Listener, FIONBIO, &ulBlock) == SOCKET_ERROR)
		{
			strcpy(Log, "Failed ioctlsocket\n");
			return 0;
		}
		//
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
	Sleep(10);
	recvfrom(Listener, bufer, 15, 0, (struct sockaddr *)Client, Client_Length);
	*FileLength = atoi(bufer);
	sendto(Listener, "ready", 6 * sizeof(char), 0, (struct sockaddr *)Client, *Client_Length);
	bufer = new char[15];
	Sleep(10);
	recvfrom(Listener, bufer, 15, 0, (struct sockaddr *)Client, Client_Length);
	*BytesCount = atoi(bufer);
	sendto(Listener, "ready", 6 * sizeof(char), 0, (struct sockaddr *)Client, *Client_Length);    
}

void GetFileLengthAndStartPositionByTCP(SOCKET Listener, int *FileLength, int *BytesCount, char* FileName)
{
	char *bufer = new char[15];
	recv(Listener, bufer, 15, 0);
	*FileLength = atoi(bufer);
	FILE *file;
	bufer = new char[15];
	file = fopen(FileName,"rb");
	if(file != NULL)
	{
		*BytesCount = filelength(fileno(file));
		fclose(file);
	}
	else
	{
		*BytesCount = 0;
	}
	itoa(*BytesCount, bufer, 10);
	send(Listener, bufer, 15, 0);
}

void GetFileLengthAndStartPositionByUDP(SOCKET Listener, int *FileLength, int *BytesCount, char* FileName, sockaddr_in* Client, int* Client_Length)
{
	char *bufer = new char[15];
	Sleep(10);
	recvfrom(Listener, bufer, 15, 0, (struct sockaddr *)Client, Client_Length);
	*FileLength = atoi(bufer);
	FILE *file;
	bufer = new char[15];
	file = fopen(FileName,"rb");
	if(file != NULL)
	{
		*BytesCount = filelength(fileno(file));
		fclose(file);
	}
	else
	{
		*BytesCount = 0;
	}
	itoa(*BytesCount, bufer, 10);
	Sleep(10);
	sendto(Listener, bufer, 15, 0, (struct sockaddr *)Client, *Client_Length);    
	Sleep(10);
}

string ConvertIntToString(int Number)
{
	if (Number == 0)
	{
		return "0";
	}
	string Temp="";	
	while (Number > 0)
	{
		Temp += Number % 10 + 48;
		Number /= 10;
	}
	string Result="";
	for (int i=0; i < Temp.length(); i++)
	{
		Result += Temp[Temp.length()-i-1];
	}
	return Result;
=======
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <time.h>
using namespace std;


void RunTCPServer(char** argv);
void RunUDPServer(char** argv);
int StartTCPServer(SOCKET *ClientSocket, SOCKET *Listener, char* IpAddress, char* Port, char* Log);
int StartUDPServer(SOCKET *ClientSocket, SOCKET *Listener, char* IpAddress, char* Port, char* Log);
char *GetFileNameByTCP(SOCKET Listener);
char *GetFileNameByUDP(SOCKET Listener, sockaddr_in* Client, int* Client_Length);
void GetFileLengthAndStartPositionByTCP(SOCKET Listener, int *FileLength, int *BytesCount);
void GetFileLengthAndStartPositionByUDP(SOCKET Listener, int *FileLength, int *BytesCount, sockaddr_in* Client, int* Client_Length);
void GetFileLengthAndStartPositionByTCP(SOCKET Listener, int *FileLength, int *BytesCount, char* FileName);
void GetFileLengthAndStartPositionByUDP(SOCKET Listener, int *FileLength, int *BytesCount, char* FileName, sockaddr_in* Client, int* Client_Length);
string ConvertIntToString(int Number);

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
	if(argc == 3)
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
			{
				if(!strcmp(argv[3], "UDP"))
				{                                        
					RunUDPServer(argv);
				}
			}
			getch();
		}
	}
	return 0;
}

void RunTCPServer(char** argv)
{        
	while(true)
	{                
		char* Log = new char[100];
		SOCKET ClientSocket, Listener;     		
		StartTCPServer(&ClientSocket, &Listener, argv[1], argv[2], Log);

		//
		int Number = 0;
		FD_SET ReadSet;
		int ReadySock;
		map <int,string> clients;
		clients.clear();
		typedef map<int,string>::value_type valType;
		//

		/*char FileName[106];
		strcpy(FileName, GetFileNameByTCP(ClientSocket));
		int BytesCount = 0, FileLength = 0;
		GetFileLengthAndStartPositionByTCP(ClientSocket, &FileLength, &BytesCount);*/
		while(true)
		{
			FILE *file;
			char bufer[65536];
			int Response = 0;

			//
			// Заполняем множество сокетов
			FD_ZERO(&ReadSet);
			FD_SET(Listener, &ReadSet);
			for(map<int,string>::iterator it = clients.begin(); it != clients.end(); it++)
				FD_SET((*it).first , &ReadSet);
			timeval timeout;
			timeout.tv_sec = 15;
			timeout.tv_usec = 0;
			int mx = Listener;
			for(map<int,string>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				if ((*it).first > Listener)
					mx = (*it).first;
			};
			// Ждём события в одном из сокетов
			if ((ReadySock = select(mx+1, &ReadSet, NULL, NULL, &timeout)) == SOCKET_ERROR)
			{
				printf("Select Error\n");
				return;
			}
			if (FD_ISSET(Listener, &ReadSet))
			{
				ClientSocket = accept(Listener, NULL, NULL);
				if (ClientSocket == INVALID_SOCKET)
				{
					return;
				}
				ULONG ulBlock = 1;
				if (ioctlsocket(ClientSocket, FIONBIO, &ulBlock) == SOCKET_ERROR)
				{
					return;
				}
				Sleep(1);
				char FileName[106];
				strcpy(FileName, GetFileNameByTCP(ClientSocket));
				int BytesCount = 0, FileLength = 0;
				GetFileLengthAndStartPositionByTCP(ClientSocket, &FileLength, &BytesCount, FileName);
				Number++;   // увеличиваем счетчик подключившихся клиентов
				string filename;//(ConvertIntToString(Number));
				filename.append(FileName);
				clients.insert( valType(ClientSocket,filename));
			}
			for(map<int,string>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				if(FD_ISSET((*it).first, &ReadSet))
				{
					FILE *F;
					if (!(F = fopen((*it).second.c_str() ,"ab")))
					{
						perror("Create File");
						break;
					};

					Response = recv((*it).first, bufer, 65536, 0);
					if(Response <= 0)
					{
						// Соединение разорвано, удаляем сокет из множества
						closesocket((*it).first);
						clients.erase((*it).first);						
						if(clients.size() == 0)
						{
							break;
						}
						else
						{
							it = clients.begin();
							continue;
						}
					}
					//записали данные в файл
					fwrite(bufer, sizeof(char), Response, F);
					// Отправляем данные обратно клиенту
					send((*it).first, "ready", 6 * sizeof(char), 0);

					fclose(F);
				}
				//printf("\nReceiving the part of file %d is complete.\n",Number);
			}    
			//

			/*file = fopen(FileName, "ab");
			Response = recv(ClientSocket, bufer, 1000, 0);
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
			fclose (file);*/
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
		SOCKET ClientSocket, Listener;                                
		StartUDPServer(&ClientSocket, &Listener, argv[1], argv[2], Log);

		//
		int Number = 0;
		FD_SET ReadSet;
		int ReadySock;
		map <int,string> clients;
		clients.clear();
		typedef map<int,string>::value_type valType;
		//

		/*char FileName[106];
		strcpy(FileName, GetFileNameByUDP(ClientSocket, &Client, &Client_Length));
		int BytesCount = 0, FileLength = 0;
		GetFileLengthAndStartPositionByUDP(ClientSocket, &FileLength, &BytesCount, &Client, &Client_Length);*/
		while(true)
		{
			FILE *file;
			char bufer[65536];
			int Response = 0;

			//
			// Заполняем множество сокетов
			FD_ZERO(&ReadSet);
			FD_SET(Listener, &ReadSet);
			for(map<int,string>::iterator it = clients.begin(); it != clients.end(); it++)
				FD_SET((*it).first , &ReadSet);
			timeval timeout;
			timeout.tv_sec = 15;
			timeout.tv_usec = 0;
			int mx = Listener;
			for(map<int,string>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				if ((*it).first > Listener)
					mx = (*it).first;
			};
			// Ждём события в одном из сокетов
			if ((ReadySock = select(mx+1, &ReadSet, NULL, NULL, &timeout)) == SOCKET_ERROR)
			{
				printf("Select Error\n");
				return;
			}
			if (FD_ISSET(Listener, &ReadSet))
			{
				ClientSocket = Listener;
				ULONG ulBlock = 1;
				if (ioctlsocket(ClientSocket, FIONBIO, &ulBlock) == SOCKET_ERROR)
				{
					return;
				}
				Sleep(1);
				char FileName[106];
				strcpy(FileName, GetFileNameByUDP(ClientSocket, &Client, &Client_Length));
				int BytesCount = 0, FileLength = 0;
				GetFileLengthAndStartPositionByUDP(ClientSocket, &FileLength, &BytesCount, FileName, &Client, &Client_Length);
				Number++;   // увеличиваем счетчик подключившихся клиентов
				string filename;//(ConvertIntToString(Number));
				filename.append(FileName);
				clients.insert( valType(ClientSocket,filename));
			}
			for(map<int,string>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				if(FD_ISSET((*it).first, &ReadSet))
				{
					ClientSocket = Listener;    
					FILE *F;
					if (!(F = fopen((*it).second.c_str() ,"ab")))
					{
						perror("Create File");
						break;
					};
					Response = recvfrom(ClientSocket, bufer, 65536, 0, (struct sockaddr *)&Client, &Client_Length);
					if(Response <= 0)
					{
						// Соединение разорвано, удаляем сокет из множества
						closesocket((*it).first);
						clients.erase((*it).first);						
						if(clients.size() == 0)
						{
							break;
						}
						else
						{
							it = clients.begin();
							continue;
						}
					}
					//записали данные в файл
					fwrite(bufer, sizeof(char), Response, F);
					// Отправляем данные обратно клиенту
					sendto(ClientSocket, "ready", 6 * sizeof(char), 0, (struct sockaddr *)&Client, Client_Length);

					fclose(F);
				}
				//printf("\nReceiving the part of file %d is complete.\n",Number);
			}    
			//

			/*file = fopen(FileName, "ab");
			Response = recvfrom(ClientSocket, bufer, 1000, 0, (struct sockaddr *)&Client, &Client_Length);
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
			fclose (file);*/
		}
	}
}

int StartTCPServer(SOCKET *ClientSocket, SOCKET *Listener, char* IpAddress, char* Port, char* Log)
{
	sockaddr_in ListenerName;
	int Result = SocketHelper::InitializeSocket(Listener, &ListenerName, SOCK_STREAM, inet_addr(IpAddress), htons(atoi(Port)), Log);
	if(!Result)
	{
		int Answer = bind(*Listener, (const sockaddr*)&ListenerName, sizeof(ListenerName));                
		if (Answer != 0)
		{
			strcpy(Log, "Failed to bind socket\n");
			return 0;
		}

		//
		ULONG ulBlock = 1;
		if (ioctlsocket(*Listener, FIONBIO, &ulBlock) == SOCKET_ERROR)
		{
			strcpy(Log, "Failed ioctlsocket\n");
			return 0;
		}
		//

		Answer = listen(*Listener, SOMAXCONN);
		if (Answer != 0)
		{
			strcpy(Log, "Failed to put socket into listening state\n");
			return 0;
		}
		//*ClientSocket = accept(Listener, NULL, NULL);
	}
	return Result;
}

int StartUDPServer(SOCKET *ClientSocket, SOCKET *Listener, char* IpAddress, char* Port, char* Log)
{
	sockaddr_in ListenerName;        
	int Result = SocketHelper::InitializeSocket(Listener, &ListenerName, SOCK_DGRAM, inet_addr(IpAddress), htons(atoi(Port)), Log);        
	if(!Result)
	{
		int Answer = bind(*Listener, (const sockaddr*)&ListenerName, sizeof(ListenerName));        
		if (Answer != 0)
		{
			strcpy(Log, "Failed to bind socket\n");
			return 0;
		}                        

		//
		ULONG ulBlock = 1;
		if (ioctlsocket(*Listener, FIONBIO, &ulBlock) == SOCKET_ERROR)
		{
			strcpy(Log, "Failed ioctlsocket\n");
			return 0;
		}
		//
		//*ClientSocket = Listener;                
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
	Sleep(10);
	recvfrom(Listener, bufer, 15, 0, (struct sockaddr *)Client, Client_Length);
	*FileLength = atoi(bufer);
	sendto(Listener, "ready", 6 * sizeof(char), 0, (struct sockaddr *)Client, *Client_Length);
	bufer = new char[15];
	Sleep(10);
	recvfrom(Listener, bufer, 15, 0, (struct sockaddr *)Client, Client_Length);
	*BytesCount = atoi(bufer);
	sendto(Listener, "ready", 6 * sizeof(char), 0, (struct sockaddr *)Client, *Client_Length);    
}

void GetFileLengthAndStartPositionByTCP(SOCKET Listener, int *FileLength, int *BytesCount, char* FileName)
{
	char *bufer = new char[15];
	recv(Listener, bufer, 15, 0);
	*FileLength = atoi(bufer);
	FILE *file;
	bufer = new char[15];
	file = fopen(FileName,"rb");
	if(file != NULL)
	{
		*BytesCount = filelength(fileno(file));
		fclose(file);
	}
	else
	{
		*BytesCount = 0;
	}
	itoa(*BytesCount, bufer, 10);
	send(Listener, bufer, 15, 0);
}

void GetFileLengthAndStartPositionByUDP(SOCKET Listener, int *FileLength, int *BytesCount, char* FileName, sockaddr_in* Client, int* Client_Length)
{
	char *bufer = new char[15];
	Sleep(10);
	recvfrom(Listener, bufer, 15, 0, (struct sockaddr *)Client, Client_Length);
	*FileLength = atoi(bufer);
	FILE *file;
	bufer = new char[15];
	file = fopen(FileName,"rb");
	if(file != NULL)
	{
		*BytesCount = filelength(fileno(file));
		fclose(file);
	}
	else
	{
		*BytesCount = 0;
	}
	itoa(*BytesCount, bufer, 10);
	Sleep(10);
	sendto(Listener, bufer, 15, 0, (struct sockaddr *)Client, *Client_Length);    
	Sleep(10);
}

string ConvertIntToString(int Number)
{
	if (Number == 0)
	{
		return "0";
	}
	string Temp="";	
	while (Number > 0)
	{
		Temp += Number % 10 + 48;
		Number /= 10;
	}
	string Result="";
	for (int i=0; i < Temp.length(); i++)
	{
		Result += Temp[Temp.length()-i-1];
	}
	return Result;
>>>>>>> f5d9b8976f78b429b32d2fc74410c523795933b1:SPOLKS/SPOLKS_Lab3-7/Server/Server/main.cpp
}