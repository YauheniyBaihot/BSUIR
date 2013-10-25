#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include "Server.h"

int InitializeSocket(SOCKET *Listener, sockaddr_in *ListenerName, ULONG IPAddress, USHORT Port);

int main(int argc, char** argv)
{
	int rc, i=1;	
	FILE *f;
	Server *S = new Server();
	S->StartServer(*argv);
	

	while (true)//бесконечный цикл
	{
		f = fopen("D:\\newinfo.txt","ab");//если его нет, перед открытием newinfo.txt создается 
		char buf[1024];
		int r = recv(*S->ClientSocket, buf, 1024, 0);//прием файла от клиента. Функция возвращает число считанных байтов 
		if (r <= 0)//если нет данных
		{
			puts("0 bytes");
			printf("%dError: \n", WSAGetLastError());
			return false;//выход из цикла
		}
		fwrite(buf,1,r,f);
		//WriteFile(f, buf, r, NULL, NULL);// указатель на файл, буфер, из которого берутся данные, байт для записи, указатель на количество записанных байт 
		//передача ответа после приема 
		//printf("\n%s \n",buf);
		printf("receive bytes: %d, part: %d\n\n",r,i);
		send(*S->ClientSocket,"ready",6*sizeof(char),0);
		i++;
		fclose (f);
	}
	getch();
	//размыкание соединения
	shutdown(*S->ClientSocket, 2);
	//заканчивает использование ws2_32.dll
	WSACleanup();
	return 0;
}

int InitializeSocket(SOCKET *Listener, sockaddr_in *ListenerName, ULONG IPAddress, USHORT Port)
{
	WSADATA WsaData;
	int WSAStartupReturnValue = WSAStartup( MAKEWORD( 2, 2 ), &WsaData );
	if ( WSAStartupReturnValue != 0 )
	{
		printf("Can't find usable wsock32.dll\n");
		return 0;
	}
	if ( LOBYTE( WsaData.wVersion ) != 2 || HIBYTE( WsaData.wVersion ) != 2 )
	{
		printf("Wsock32.dll of wrong version\n");
		return 0; 
	}
	*Listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*Listener == 0)
	{
		printf("Failed to create socket\n");
		return 0;
	}
	ListenerName->sin_family = AF_INET;
	ListenerName->sin_addr.S_un.S_addr = IPAddress;
	ListenerName->sin_port = Port;
	return 1;
}