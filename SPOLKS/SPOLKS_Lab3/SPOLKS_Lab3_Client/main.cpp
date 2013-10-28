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
	int rc, size, y=0, i=1; 
	Client *client = new Client();
	char* Log = new char[100];
	FILE *file;
	if(!client->StartClient(argv[1], argv[2], Log))
	{
		//printf("Connected to %s %s\n", argv[1], argv[2]);
		printf("Connected to 127.0.0.1 4003\n");


		file = fopen("log.txt", "r");
		char *bufer = new char[1];
		if(file != NULL)
		{			
			fread(bufer,1,1,file);
			fclose(file);
		}		

		char InformationBuffer[1024];
		client->Recv(InformationBuffer, 1024);
		if(atoi(bufer) != 0)
		{
			client->Send(bufer, strlen(bufer) * sizeof(char));
		}
		else
		{
			client->Send("ready", 6*sizeof(char));
		}

		string InformationData = string(InformationBuffer);
		int FlagPosition1 = 0, FlagPosition2 = 0;

		FlagPosition1 = InformationData.find_first_of('#', FlagPosition1);
		char *FileName = new char[FlagPosition1];
		copy(InformationData.begin(), InformationData.begin() + FlagPosition1, FileName);
		FileName[FlagPosition1] = '\0';

		FlagPosition2 = InformationData.find_first_of('#', FlagPosition1 + 1);
		char *FileLengthChar = new char[FlagPosition2 - FlagPosition1 - 1];
		copy(InformationData.begin() + FlagPosition1 + 1, InformationData.begin() + FlagPosition2, FileLengthChar);
		FileLengthChar[FlagPosition2 - FlagPosition1 - 1] = '\0';

		int FileLengthInt = atoi(FileLengthChar);
		int ByteCount = 0;
		while(true)
		{
			
			file = fopen("D:\\newinfo.txt", "ab");//если его нет, перед открытием newinfo.txt создается 
			char buf[2];
			int r = client->Recv(buf, 2);
			if (r <= 0)//если нет данных
			{
				puts("0 bytes");
				printf("%dError: \n", WSAGetLastError());
				client->CloseClient();
				fclose(file);

				if(FileLengthInt != ByteCount)
				{
					file = fopen("log.txt", "a");
					itoa(ByteCount, FileLengthChar, 10);
					fwrite(FileLengthChar, 1, strlen(FileLengthChar), file);
					fclose(file);
				}

				break;
			}
			fwrite(buf,1,r,file);

			ByteCount += r;

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
	//client->CloseClient();
	return 0;
}

