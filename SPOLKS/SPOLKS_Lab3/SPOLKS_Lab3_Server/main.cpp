#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <stdio.h>
#include <conio.h>

int main(int argc, char** argv)
{
	int rc, i=1;
	// инициализация библиотеки winsock2
	WSADATA wsd;
	//инициализирует библиотеку ws2_32.dll для приложения. версия windows socket,указатель на структуру
	WSAStartup(0X0101, &wsd);
	FILE *f;
	//инициализация сокета в случае сервера/клиента TCP. Интернет, тип сокета TCP, протокол
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

	//структура для инициализация сервера
	struct sockaddr_in myCom;
	//используемый домен
	myCom.sin_family = AF_INET;
	//порт для прослушивания
	myCom.sin_port   = htons(1200);
	//адрес привязки порта
	myCom.sin_addr.S_un.S_addr = INADDR_ANY;

	//закрепляем порт за приложением. прослушивающий сокет, порт и сетевой интерфейс из структуры, размер
	rc = bind(s,(struct sockaddr*)&myCom,sizeof(myCom));
	if (rc<0) printf ("error");
	//прослушивается сокет
	listen(s, 5);

	//принятие соединения, вызов сокета
	SOCKET s1; 
	if ((s1=accept(s,NULL,NULL))==-1)printf ("error");

	while (true)//бесконечный цикл
	{
		f = fopen("E:\\newinfo.txt","ab");//если его нет, перед открытием newinfo.txt создается 
		char buf[100];
		int r = recv(s1, buf, sizeof(buf), 0);//прием файла от клиента. Функция возвращает число считанных байтов 
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
		send(s1,"ready",6*sizeof(char),0);
		i++;
		fclose (f);
	}
	getch();
	//размыкание соединения
	shutdown(s1, 2);
	//заканчивает использование ws2_32.dll
	WSACleanup();
	return 0;
}

