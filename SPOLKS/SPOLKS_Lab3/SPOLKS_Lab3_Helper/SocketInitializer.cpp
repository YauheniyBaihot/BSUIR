#include "SocketInitializer.h"


int SocketInitializer::InitializeSocket(SOCKET *Listener, sockaddr_in *ListenerName, ULONG IPAddress, USHORT Port, char* Log)
{
	WSADATA WsaData;
	int WSAStartupReturnValue = WSAStartup( MAKEWORD( 2, 2 ), &WsaData );
	if ( WSAStartupReturnValue != 0 )
	{
		Log = "Can't find usable wsock32.dll\n";
		return 1;
	}
	if ( LOBYTE( WsaData.wVersion ) != 2 || HIBYTE( WsaData.wVersion ) != 2 )
	{
		Log = "Wsock32.dll of wrong version\n";
		return 2; 
	}
	*Listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*Listener == 0)
	{
		Log = "Failed to create socket\n";
		return 3;
	}
	ListenerName->sin_family = AF_INET;
	ListenerName->sin_addr.S_un.S_addr = IPAddress;
	ListenerName->sin_port = Port;
	return 0;
}