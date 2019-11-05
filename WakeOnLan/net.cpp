#include "net.h"

bool net_init()
{
	WSADATA data;
	return (bool)!WSAStartup(MAKEWORD(2,1),&data);
}

bool net_quit()
{
	return (bool)!WSACleanup();
}
