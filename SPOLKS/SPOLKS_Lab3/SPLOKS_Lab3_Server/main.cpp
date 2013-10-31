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
bool KeyPressed(int key);

int main(int argc, char** argv)
{
	while(true)
	{
		int y, i=1, symbols, size;	
		char* Log = new char[100];
		SOCKET ClientSocket;		
		StartServer(&ClientSocket, "127.0.0.1", "1200", Log);

		char *FilePath = "D:\\for_send.txt";
		FILE *file;
		file = fopen(FilePath,"r");


		if(file != NULL)
		{			
			char buf[1024];

			char *FileName = new char[100];
			char *Extension = new char[5];
			_splitpath(FilePath, new char[1], new char[200], FileName, Extension);
			strcat(FileName, Extension);
			strcat(FileName, "#");
			send(ClientSocket, FileName, strlen(FileName) + 1, 0);
			recv(ClientSocket, buf, sizeof(buf), 0);


			char *FileLength = new char[15];
			itoa(filelength(fileno(file)), FileLength, 10);		
			send(ClientSocket, FileLength, strlen(FileLength), 0);
			recv(ClientSocket, buf, sizeof(buf), 0);
			fseek(file, atoi(buf), SEEK_SET);

			while(!feof(file)) 
			{
				char bufer[2];
				if(KeyPressed(VK_UP))
				{
					int result=send(ClientSocket,"~",1,MSG_OOB);
					printf("send out of band data");
					//Sleep(500);
				}	
				else
				{
					symbols=fread(bufer,1,2,file);
					size=ftell(file);
					//printf("\n%s \n",bufer);
					printf("read symbols: %d, part: %d, pos: %ld \n",symbols,i,size);
					printf("\n");	
					send(ClientSocket, bufer, symbols, 0);
					//Sleep(500);
					i++;
				}
				y = recv(ClientSocket, buf, sizeof(buf), 0);
				if(y > 0)
				{
					printf("%s\n",buf);
				}
				else
				{					
					break;
				}							
			}	
			fclose (file);
			SocketHelper::CloseSocket(ClientSocket);
		}		
	}
	getch();
	return 0;
}

int StartServer(SOCKET *ClientSocket, char* IpAddress, char* Port, char* Log)
{
	SOCKET Listener;
	sockaddr_in ListenerName;
	int Result = SocketHelper::InitializeSocket(&Listener, &ListenerName, INADDR_ANY, htons(4003), Log);
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

bool KeyPressed(int key)
{
	return (GetAsyncKeyState(key) & 0x8000 != 0);
}
