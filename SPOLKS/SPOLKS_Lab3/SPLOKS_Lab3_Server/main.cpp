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
			char *FileLength = new char[15];

			itoa(filelength(fileno(file)), FileLength, 10);
			
			char buf[1024];//для приема сообщений сервера			
			server->Send(FileLength, strlen(FileLength));
			server->Recv(buf, sizeof(buf));
			fseek(file, atoi(buf), SEEK_SET);
			
			while(!feof(file)) //пока не конец файла, передаем частями файл (сколько помещается в буфере)
			{
				char bufer[2];
				symbols=fread(bufer,1,2,file);//в буфер один раз считывается sizeof(bufer) бит из файла, возвращает число символов
				size=ftell(file);//функция возвращает текущую позицию
				//printf("\n%s \n",bufer);
				printf("read symbols: %d, part: %d, pos: %ld \n",symbols,i,size);
				printf("\n");
				//if(symbols!=0)
				Sleep(500);
				server->Send(bufer, symbols);
				i++;
				//if (y==0) break;
				
				y = server->Recv(buf, sizeof(buf));
				if(y > 0)
				{
					printf("%s\n",buf);
				}
				else
				{
					fclose (file);
					server->CloseServer();
					break;
				}
			}			
		}		
	}
	getch();
	return 0;
}