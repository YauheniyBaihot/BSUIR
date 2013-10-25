#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>
#include <io.h>

int main(int argc, char** argv)
{
	int rc, size, symbols, y=0, i=1; //проверка соединения, позиция при считывании файла, число считанных символов, счетчик.   
	//char *bufer;//для считывания передаваемого файла
	//long lenfile;//размер открываемого файла для передачи
	// инициализация библиотеки winsock2
	WSADATA wsd;
	//инициализирует библиотеку для приложения
	WSAStartup(0X0101, &wsd);
	//инициализация сокета в случае сервера/клиента TCP
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

	//описание структуры
	sockaddr_in peer;
	peer.sin_family = AF_INET;
	peer.sin_port   = htons(1200);
	peer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	//инициализация клиента
	rc = connect(s,(struct sockaddr*) &peer, sizeof(peer));
	if (rc<0) printf ("error");

	FILE *f;
	f = fopen("D:\\for_send.txt","r");
	//lenfile= filelength(fileno(f));//определение размера файла в байтах
	//bufer=new char[lenfile];
	if(f != NULL)
	{
	while(!feof(f)) //пока не конец файла, передаем частями файл (сколько помещается в буфере)
	{
		char bufer[100];
		symbols=fread(bufer,1,100,f);//в буфер один раз считывается sizeof(bufer) бит из файла, возвращает число символов
		size=ftell(f);//функция возвращает текущую позицию
		//printf("\n%s \n",bufer);
		printf("read symbols: %d, part: %d, pos: %ld \n",symbols,i,size);
		printf("\n");
		//if(symbols!=0)
		send(s,bufer,symbols,0);//передача содержимого буфера
		i++;
		//if (y==0) break;
		char buf[100];//для приема сообщений сервера
		y = recv(s,buf,sizeof(buf),0);//прием ответа сервера (sizeof(buf) - максимально возможный принятый объем)
		printf("%s\n",buf);
	}
	fclose (f);
	}
	getch();
	// размыкание соединения
	shutdown(s, 2);
	//заканчивает использование ws2_32.dll
	WSACleanup();
	return 0;
}

