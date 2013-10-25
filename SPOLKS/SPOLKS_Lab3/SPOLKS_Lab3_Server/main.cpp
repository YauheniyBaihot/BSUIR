#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>

int main(int argc, char** argv)
{
	int rc, i=1;
	// ������������� ���������� winsock2
	WSADATA wsd;
	//�������������� ���������� ws2_32.dll ��� ����������. ������ windows socket,��������� �� ���������
	WSAStartup(0X0101, &wsd);
	FILE *f;
	//������������� ������ � ������ �������/������� TCP. ��������, ��� ������ TCP, ��������
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

	//��������� ��� ������������� �������
	struct sockaddr_in myCom;
	//������������ �����
	myCom.sin_family = AF_INET;
	//���� ��� �������������
	myCom.sin_port   = htons(1200);
	//����� �������� �����
	myCom.sin_addr.S_un.S_addr = INADDR_ANY;

	//���������� ���� �� �����������. �������������� �����, ���� � ������� ��������� �� ���������, ������
	rc = bind(s,(struct sockaddr*)&myCom,sizeof(myCom));
	if (rc<0) printf ("error");
	//�������������� �����
	listen(s, 5);

	//�������� ����������, ����� ������
	SOCKET s1; 
	if ((s1=accept(s,NULL,NULL))==-1)printf ("error");

	while (true)//����������� ����
	{
		f = fopen("E:\\newinfo.txt","ab");//���� ��� ���, ����� ��������� newinfo.txt ��������� 
		char buf[100];
		int r = recv(s1, buf, sizeof(buf), 0);//����� ����� �� �������. ������� ���������� ����� ��������� ������ 
		if (r <= 0)//���� ��� ������
		{
			puts("0 bytes");
			printf("%dError: \n", WSAGetLastError());
			return false;//����� �� �����
		}
		fwrite(buf,1,r,f);
		//WriteFile(f, buf, r, NULL, NULL);// ��������� �� ����, �����, �� �������� ������� ������, ���� ��� ������, ��������� �� ���������� ���������� ���� 
		//�������� ������ ����� ������ 
		//printf("\n%s \n",buf);
		printf("receive bytes: %d, part: %d\n\n",r,i);
		send(s1,"ready",6*sizeof(char),0);
		i++;
		fclose (f);
	}
	getch();
	//���������� ����������
	shutdown(s1, 2);
	//����������� ������������� ws2_32.dll
	WSACleanup();
	return 0;
}

