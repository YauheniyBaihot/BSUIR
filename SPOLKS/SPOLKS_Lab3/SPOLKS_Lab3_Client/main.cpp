#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
#include "Client.h"

int ReadCountOfSendedBytesFromLogFile();
void WriteCountOfSendedBytesToLogFile(int BytesCount);


int main(int argc, char** argv)
{
	int rc, size, y=0, i=1; 
	Client *client = new Client();
	char* Log = new char[100];
	FILE *file;
	if(!client->StartClient(argv[1], argv[2], Log))
	{
		//printf("Connected to %s %s\n", argv[1], argv[2]);
		printf("Connected to 127.0.0.1 4003\n");

		char FileNameBufer[106];
		client->Recv(FileNameBufer, 106);
		string FileName(FileNameBufer);
		FileNameBufer[FileName.find_last_of('#', 105)] = '\0';
		client->Send("ready", 6 * sizeof(char));

		char FileLengthBufer[15];
		client->Recv(FileLengthBufer, 15);
		int BytesCount = ReadCountOfSendedBytesFromLogFile();
		if(BytesCount != 0)
		{
			char *bufer = new char[15];
			itoa(BytesCount, bufer, 10);
			client->Send(bufer, strlen(bufer) * sizeof(char));
		}
		else
		{
			client->Send("ready", 6 * sizeof(char));
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
			FD_SET(client->Listener, &fdread);
			FD_SET(client->Listener, &fdOOB);


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
				if(FD_ISSET(client->Listener,&fdOOB))
				{
					r=recv(client->Listener,(char *)&buf,2,MSG_OOB);
					printf("\n I receive the OOB data, NUM IS %d\n",r);
					client->Send("ready", 6*sizeof(char));            
				} 
				else
				{
					file = fopen(FileNameBufer, "ab");//если его нет, перед открытием newinfo.txt создается 


					r = client->Recv(buf, 2);
					if (r <= 0)//если нет данных
					{

						puts("0 bytes");
						printf("%dError: \n", WSAGetLastError());
						client->CloseClient();
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
					client->Send("ready", 6*sizeof(char));
					i++;
					fclose (file);
				}
			}

		}
	}
	getch();
	return 0;
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