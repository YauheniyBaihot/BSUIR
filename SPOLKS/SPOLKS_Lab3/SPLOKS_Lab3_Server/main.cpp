#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include "Server.h"

int main(int argc, char** argv)
{
	while(true)
	{
		int rc, i=1;	
		FILE *f;
		Server *S = new Server();
		S->StartServer(*argv);


		while (true)//����������� ����
		{
			f = fopen("D:\\newinfo.txt","ab");//���� ��� ���, ����� ��������� newinfo.txt ��������� 
			char buf[2];
			int r = S->Recv(buf, 2);
			if (r <= 0)//���� ��� ������
			{
				puts("0 bytes");
				printf("%dError: \n", WSAGetLastError());
				S->CloseServer();
				break;
			}
			fwrite(buf,2,r,f);
			//WriteFile(f, buf, r, NULL, NULL);// ��������� �� ����, �����, �� �������� ������� ������, ���� ��� ������, ��������� �� ���������� ���������� ���� 
			//�������� ������ ����� ������ 
			//printf("\n%s \n",buf);
			printf("receive bytes: %d, part: %d\n\n",r,i);
			S->Send("ready", 6*sizeof(char));
			i++;
			fclose (f);
		}
	}
	getch();

	return 0;
}