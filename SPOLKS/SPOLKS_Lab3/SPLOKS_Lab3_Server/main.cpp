#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#include "SocketHelper.h"

int StartServer(SOCKET *ClientSocket, char* IpAddress, char* Port, char* Log);
void SendFileName(char *FilePath, SOCKET ClientSocket);
int SendFileLength(FILE *file, SOCKET ClientSocket);
bool KeyPressed(int key);

int main(int argc, char** argv)
{
	if(argc < 4)
	{
		return 0;
	}
	printf("Server start\n");
	while(true)
	{
		int symbols;	
		char* Log = new char[100];
		SOCKET ClientSocket;			
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