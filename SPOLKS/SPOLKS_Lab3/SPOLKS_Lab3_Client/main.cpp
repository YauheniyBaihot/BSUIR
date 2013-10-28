#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <string>
#include <iostream>
using namespace std;
#include "Client.h"

int main(int argc, char** argv)
{
	int rc, size, symbols, y=0, i=1; 
	Client *client = new Client();
	char* Log = new char[100];
	if(!client->StartClient(argv[1], argv[2], Log))
	{
		//printf("Connected to %s %s\n", argv[1], argv[2]);
		printf("Connected to 127.0.0.1 1200\n");
		while(true)
		{
			FILE *file;
			file = fopen("D:\\newinfo.txt","ab");//если его нет, перед открытием newinfo.txt создается 
			char buf[2];
			int r = client->Recv(buf, 2);
			if (r <= 0)//если нет данных
			{
				puts("0 bytes");
				printf("%dError: \n", WSAGetLastError());
				client->CloseClient();
				break;
			}
			fwrite(buf,1,r,file);
			//WriteFile(file, buf, r, NULL, NULL);// указатель на файл, буфер, из которого берутся данные, байт для записи, указатель на количество записанных байт 
			//передача ответа после приема 
			//printf("\n%s \n",buf);
			printf("receive bytes: %d, part: %d\n\n",r,i);
			client->Send("ready", 6*sizeof(char));
			i++;
			fclose (file);
			/*string Command;
			getline(cin, Command);
			if(Command[0] == 's')
			{
			char *FilePath = new char[Command.length() - 2];
			copy(Command.begin() + 2, Command.end(), FilePath);
			FilePath[Command.size() - 2] = '\0';
			FILE *file;
			file = fopen(FilePath,"r");
			char *FileName = new char[100];
			char *Extension = new char[5];
			_splitpath(FilePath, new char[1], new char[200], FileName, Extension);
			strcat(FileName, Extension);
			char *FileLength = new char[15];
			itoa(filelength(fileno(file)), FileLength, 10);
			char *bufer1 = new char[strlen(FileLength) + strlen(FileName) + 1];
			bufer1[0] = '\0';
			strcat(bufer1, FileName);
			strcat(bufer1, "#");
			strcat(bufer1, FileLength);

			}*/
		}
	}

	//lenfile= filelength(fileno(file));//определение размера файла в байтах
	//bufer=new char[lenfile];
	getch();
	client->CloseClient();
	return 0;
}

