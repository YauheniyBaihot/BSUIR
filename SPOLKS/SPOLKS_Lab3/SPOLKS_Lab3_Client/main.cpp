#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include "Client.h"

int main(int argc, char** argv)
{
	int rc, size, symbols, y=0, i=1; 
	Client *C = new Client();
	C->StartClient(*argv);

	FILE *f;
	f = fopen("D:\\for_send.txt","r");
	//lenfile= filelength(fileno(f));//����������� ������� ����� � ������
	//bufer=new char[lenfile];
	if(f != NULL)
	{
		while(!feof(f)) //���� �� ����� �����, �������� ������� ���� (������� ���������� � ������)
		{
			char bufer[2];
			symbols=fread(bufer,1,2,f);//� ����� ���� ��� ����������� sizeof(bufer) ��� �� �����, ���������� ����� ��������
			size=ftell(f);//������� ���������� ������� �������
			//printf("\n%s \n",bufer);
			printf("read symbols: %d, part: %d, pos: %ld \n",symbols,i,size);
			printf("\n");
			//if(symbols!=0)
			C->Send(bufer, symbols);
			i++;
			//if (y==0) break;
			char buf[1024];//��� ������ ��������� �������
			y = C->Recv(buf, sizeof(buf));
			printf("%s\n",buf);
		}
		fclose (f);
	}
	getch();
	C->CloseClient();
	return 0;
}

