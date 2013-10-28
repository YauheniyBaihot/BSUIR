#include "Client.h"
#include "SocketHelper.h"


Client::Client(void)
{
}


Client::~Client(void)
{
}


int Client::StartClient(char* IpAddress, char* Port, char* Log)
{
	int Result = SocketHelper::InitializeSocket(&Listener, &ListenerName, inet_addr("127.0.0.1"), htons(4003), Log);
	if(!Result)
	{
		int Answer = connect(Listener, (const sockaddr*)&ListenerName, sizeof(ListenerName));
		if (Answer != 0)
		{
			strcpy(Log, "Failed to connect socket\n");
			return 0;
		}
	}
	return Result;
}


void Client::CloseClient()
{
	SocketHelper::CloseSocket(Listener);
}


int Client::Recv(char *Buffer, int Symbols)
{
	return SocketHelper::Recv(Listener, Buffer, Symbols);
}


void Client::Send(char *Buffer, int Symbols)
{
	SocketHelper::Send(Listener, Buffer, Symbols);
}