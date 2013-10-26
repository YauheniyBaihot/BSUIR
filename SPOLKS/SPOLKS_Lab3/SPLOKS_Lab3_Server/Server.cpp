#include "Server.h"
#include "SocketHelper.h"

Server::Server(void)
{

}


Server::~Server(void)
{
}


int Server::StartServer(char* Log)
{
	int Result = SocketHelper::InitializeSocket(&Listener, &ListenerName, INADDR_ANY, htons(1200), Log);
	if(!Result)
	{
		int Answer = bind(Listener, (const sockaddr*)&ListenerName, sizeof(ListenerName));
		if (Answer != 0)
		{
			Log = "Failed to bind socket\n";
			return 0;
		}
		Answer = listen(Listener,SOMAXCONN);
		if (Answer != 0)
		{
			Log = "Failed to put socket into listening state\n";
			return 0;
		}
		ClientSocket = accept(Listener, NULL, NULL);
	}
	return Result;
}


void Server::CloseServer()
{
	SocketHelper::CloseSocket(ClientSocket);
}


int Server::Recv(char *Buffer, int Symbols)
{
	return SocketHelper::Recv(ClientSocket, Buffer, Symbols);
}


void Server::Send(char *Buffer, int Symbols)
{
	SocketHelper::Send(ClientSocket, Buffer, Symbols);
}