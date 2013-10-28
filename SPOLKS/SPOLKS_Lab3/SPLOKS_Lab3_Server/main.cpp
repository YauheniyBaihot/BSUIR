#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <string>
#include <iostream>
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
			char *FileName = new char[100];
			char *Extension = new char[5];
			_splitpath(FilePath, new char[1], new char[200], FileName, Extension);
			strcat(FileName, Extension);
			char *FileLength = new char[15];
			itoa(filelength(fileno(file)), FileLength, 10);
			char *InformationBuffer = new char[strlen(FileLength) + strlen(FileName) + 2];
			InformationBuffer[0] = '\0';
			strcat(InformationBuffer, FileName);
			strcat(InformationBuffer, "#");
			strcat(InformationBuffer, FileLength);
			strcat(InformationBuffer, "#");

			char buf[1024];//для приема сообщений сервера			
			server->Send(InformationBuffer, strlen(InformationBuffer));
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
				printf("%s\n",buf);
			}
			fclose (file);
		}
		//while (true)//бесконечный цикл
		//{
		//	file = fopen("D:\\newinfo.txt","a");//если его нет, перед открытием newinfo.txt создается 
		//	char buf[2];
		//	int r = server->Recv(buf, 2);
		//	if (r <= 0)//если нет данных
		//	{
		//		puts("0 bytes");
		//		printf("%dError: \n", WSAGetLastError());
		//		server->CloseServer();
		//		break;
		//	}
		//	fwrite(buf,1,r,file);
		//	//WriteFile(file, buf, r, NULL, NULL);// указатель на файл, буфер, из которого берутся данные, байт для записи, указатель на количество записанных байт 
		//	//передача ответа после приема 
		//	//printf("\n%s \n",buf);
		//	printf("receive bytes: %d, part: %d\n\n",r,i);
		//	server->Send("ready", 6*sizeof(char));
		//	i++;
		//	fclose (file);
		//}
		server->CloseServer();
	}
	getch();
	return 0;
}