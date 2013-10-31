#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
#include "SocketHelper.h"

int StartClient(SOCKET *Listener, char* IpAddress, char* Port, char* Log);
int ReadCountOfSendedBytesFromLogFile();
void WriteCountOfSendedBytesToLogFile(int BytesCount);

int main(int argc, char** argv)
{
	int rc, size, y=0, i=1; 
	char* Log = new char[100];
	FILE *file;
	SOCKET Listener;
	if(!StartClient(&Listener, argv[1], argv[2], Log))
	{
		//printf("Connected to %s %s\n", argv[1], argv[2]);
		printf("Connected to 127.0.0.1 4003\n");

		char FileNameBufer[106];
		recv(Listener, FileNameBufer, 106, 0);
		string FileName(FileNameBufer);
		FileNameBufer[FileName.find_last_of('#', 105)] = '\0';
		send(Listener, "ready", 6 * sizeof(char), 0);

		char FileLengthBufer[15];
		recv(Listener, FileLengthBufer, 15, 0);
		int BytesCount = ReadCountOfSendedBytesFromLogFile();
		if(BytesCount != 0)
		{
			char *bufer = new char[15];
			itoa(BytesCount, bufer, 10);
			send(Listener, bufer, strlen(bufer) * sizeof(char), 0);
		}
		else
		{
			send(Listener, "ready", 6 * sizeof(char), 0);
		}


		int FileLength = atoi(FileLengthBufer);

		while(true)
		{
			char buf[2];
			int r = 0;

			fd_set fdread,fdOOB;
			BOOL isOOB = false;
			FD_ZERO(&fdread);
			FD_ZERO(&fdOOB);
			// 'remoteSocket' is the socket that you check in this sample.
			FD_SET(Listener, &fdread);
			FD_SET(Listener, &fdOOB);


			/*u_long Mode = 0;
			bool vv = ioctlsocket(client->Listener, SIOCATMARK, &Mode);
			if(vv)
			{
			char c[512];
			int nbytes = recv(client->Listener, c, 1,MSG_OOB);
			printf("oob data %d", nbytes);
			}*/
			int err = 0;
			if((err=select(0,&fdread,0,&fdOOB,0))==SOCKET_ERROR)
			{
				printf("select() failed: %d\n",WSAGetLastError());
				return 0;
			}
			else
			{
				if(FD_ISSET(Listener,&fdOOB))
				{
					char bbb[1];
					recv(Listener,bbb,1,MSG_OOB);
					printf("\n I receive the OOB data, NUM IS %s\n",bbb);
					send(Listener, "ready", 6 * sizeof(char), 0);     
				} 
				else
				{
					file = fopen(FileNameBufer, "ab");//если его нет, перед открытием newinfo.txt создается 


					r = recv(Listener, buf, 2, 0);
					if (r <= 0)//если нет данных
					{

						puts("0 bytes");
						printf("%dError: \n", WSAGetLastError());
						SocketHelper::CloseSocket(Listener);
						fclose(file);

						if(FileLength != BytesCount)
						{
							WriteCountOfSendedBytesToLogFile(BytesCount);					
						}

						break;
					}
					fwrite(buf,1,r,file);

					BytesCount += r;


					printf("receive bytes: %d, part: %d\n\n",r,i);
					send(Listener, "ready", 6 * sizeof(char), 0);
					i++;
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
	int Result = SocketHelper::InitializeSocket(Listener, &ListenerName, inet_addr("127.0.0.1"), htons(4003), Log);
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