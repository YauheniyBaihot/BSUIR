#include "Server.h"
#include "SocketInitializer.h"

Server::Server(void)
{

}


Server::~Server(void)
{
}


int Server::StartServer(char* Log)
{
	int Result = SocketInitializer::InitializeSocket(&Listener, &ListenerName, INADDR_ANY, htons(1200), Log);
	if(!Result)
	{
		int ret = bind(Listener, (const sockaddr*)&ListenerName, sizeof(ListenerName));
		if (ret != 0)
		{
			Log = "Failed to bind socket\n";
			return 0;
		}
		ret = listen(Listener,SOMAXCONN);
		if (ret != 0)
		{
			Log = "Failed to put socket into listening state\n";
			return 0;
		}
		ClientSocket = (SOCKET*)malloc(sizeof(SOCKET));
		*ClientSocket = accept(Listener, NULL, NULL);
	}
	return Result;
}