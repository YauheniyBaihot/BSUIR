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

			char buf[1024];//��� ������ ��������� �������			
			server->Send(InformationBuffer, strlen(InformationBuffer));
			server->Recv(buf, sizeof(buf));
			fseek(file, atoi(buf), SEEK_SET);
			
			while(!feof(file)) //���� �� ����� �����, �������� ������� ���� (������� ���������� � ������)
			{
				char bufer[2];
				symbols=fread(bufer,1,2,file);//� ����� ���� ��� ����������� sizeof(bufer) ��� �� �����, ���������� ����� ��������
				size=ftell(file);//������� ���������� ������� �������
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
		//while (true)//����������� ����
		//{
		//	file = fopen("D:\\newinfo.txt","a");//���� ��� ���, ����� ��������� newinfo.txt ��������� 
		//	char buf[2];
		//	int r = server->Recv(buf, 2);
		//	if (r <= 0)//���� ��� ������
		//	{
		//		puts("0 bytes");
		//		printf("%dError: \n", WSAGetLastError());
		//		server->CloseServer();
		//		break;
		//	}
		//	fwrite(buf,1,r,file);
		//	//WriteFile(file, buf, r, NULL, NULL);// ��������� �� ����, �����, �� �������� ������� ������, ���� ��� ������, ��������� �� ���������� ���������� ���� 
		//	//�������� ������ ����� ������ 
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