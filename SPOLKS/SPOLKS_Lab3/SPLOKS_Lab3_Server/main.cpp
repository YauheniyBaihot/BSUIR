#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#include "Server.h"

bool KeyPressed(int key);

int main(int argc, char** argv)
{
	while(true)
	{
		int y, i=1, symbols, size;	

		Server *server = new Server();
		char* Log = new char[100];
		server->StartServer("127.0.0.1", "1200", Log);

		char *FilePath = "D:\\for_send.txt";
		FILE *file;
		file = fopen(FilePath,"r");


		if(file != NULL)
		{			
			char buf[1024];//для приема сообщений сервера	

			char *FileName = new char[100];
			char *Extension = new char[5];
			_splitpath(FilePath, new char[1], new char[200], FileName, Extension);
			strcat(FileName, Extension);
			strcat(FileName, "#");
			server->Send(FileName, strlen(FileName) + 1);
			server->Recv(buf, sizeof(buf));

			char *FileLength = new char[15];
			itoa(filelength(fileno(file)), FileLength, 10);					
			server->Send(FileLength, strlen(FileLength));
			server->Recv(buf, sizeof(buf));
			fseek(file, atoi(buf), SEEK_SET);

			while(!feof(file)) //пока не конец файла, передаем частями файл (сколько помещается в буфере)
			{
				char bufer[2];
				if(KeyPressed(VK_UP))
				{
					int result=send(server->ClientSocket,"AA",2,0);
					printf("send out of band data");
					Sleep(500);
				}	
				else
				{
					symbols=fread(bufer,1,2,file);//в буфер один раз считывается sizeof(bufer) бит из файла, возвращает число символов
					size=ftell(file);//функция возвращает текущую позицию
					//printf("\n%s \n",bufer);
					printf("read symbols: %d, part: %d, pos: %ld \n",symbols,i,size);
					printf("\n");
					//if(symbols!=0)					
					server->Send(bufer, symbols);
					Sleep(500);
					i++;
				}

				y = server->Recv(buf, sizeof(buf));
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
			server->CloseServer();
		}		
	}
	getch();
	return 0;
}

bool KeyPressed(int key)
{
	return (GetAsyncKeyState(key) & 0x8000 != 0);
}