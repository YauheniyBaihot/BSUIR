#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>

int main(int argc, char** argv)
{
	int rc, size, symbols, y=0, i=1; //�������� ����������, ������� ��� ���������� �����, ����� ��������� ��������, �������.   
	//char *bufer;//��� ���������� ������������� �����
	//long lenfile;//������ ������������ ����� ��� ��������
	// ������������� ���������� winsock2
	WSADATA wsd;
	//�������������� ���������� ��� ����������
	WSAStartup(0X0101, &wsd);
	//������������� ������ � ������ �������/������� TCP
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

	//�������� ���������
	sockaddr_in peer;
	peer.sin_family = AF_INET;
	peer.sin_port   = htons(1200);
	peer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	//������������� �������
	rc = connect(s,(struct sockaddr*) &peer, sizeof(peer));
	if (rc<0) printf ("error");

	FILE *f;
	f = fopen("D:\\for_send.txt","r");
	//lenfile= filelength(fileno(f));//����������� ������� ����� � ������
	//bufer=new char[lenfile];
	if(f != NULL)
	{
	while(!feof(f)) //���� �� ����� �����, �������� ������� ���� (������� ���������� � ������)
	{
		char bufer[100];
		symbols=fread(bufer,1,100,f);//� ����� ���� ��� ����������� sizeof(bufer) ��� �� �����, ���������� ����� ��������
		size=ftell(f);//������� ���������� ������� �������
		//printf("\n%s \n",bufer);
		printf("read symbols: %d, part: %d, pos: %ld \n",symbols,i,size);
		printf("\n");
		//if(symbols!=0)
		send(s,bufer,symbols,0);//�������� ����������� ������
		i++;
		//if (y==0) break;
		char buf[100];//��� ������ ��������� �������
		y = recv(s,buf,sizeof(buf),0);//����� ������ ������� (sizeof(buf) - ����������� ��������� �������� �����)
		printf("%s\n",buf);
	}
	fclose (f);
	}
	getch();
	// ���������� ����������
	shutdown(s, 2);
	//����������� ������������� ws2_32.dll
	WSACleanup();
	return 0;
}

