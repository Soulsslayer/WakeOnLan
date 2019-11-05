#ifndef NET_H
#define NET_H

#if (defined(_WIN32) || _WIN64)
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#endif 


typedef unsigned char net_byte;

extern bool net_init();
extern bool net_quit();

#endif // !NET_H

