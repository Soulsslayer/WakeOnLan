#pragma comment(lib,"ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#define inet_pton InetPton

#define PACKET_BUF         17*6

#define MAC_ADDR_MAX       6
#define MAC_ADDR_STR_MAX   64

#define CONVERT_BASE       16

#define ADDR_LEN           16
#define REMOTE_ADDR        "255.255.255.255"
#define REMOTE_PORT        9

#define ARGS_BUF_MAX       128

#define USAGE              "Usage: %s [-r remoteaddr] [-f filename1, ...|mac1, ...]\n"

typedef struct
{
	unsigned char mac_addr[MAC_ADDR_MAX];
	char mac_addr_str[MAC_ADDR_STR_MAX];
} mac_addr_t;

typedef struct
{
	char remote_addr[ADDR_LEN];
	mac_addr_t* mac_addr;
} wol_header_t;


int packMacAddr(const char* mac, mac_addr_t* packedMac)
{
	char* tmpMac = (char*)malloc(strlen(mac) * sizeof(char));
	char* delimiter = (char*)":- ";
	char* tok;
	int i;

	if (tmpMac == NULL)
	{
		fprintf(stderr, "Cannot allocate memory for mac address: %s ...!\n", strerror(errno));
		return -1;
	}

	strncpy(tmpMac, mac, strlen(mac));
	tok = strtok(tmpMac, delimiter);

	for (i = 0; i < MAC_ADDR_MAX; i++)
	{
		if (tok == NULL)
		{
			return -1;
		}

		packedMac->mac_addr[i] = (unsigned char)strtol(tok, NULL, CONVERT_BASE);
		tok = strtok(NULL, delimiter);
	}

	strncpy(packedMac->mac_addr_str, mac, MAC_ADDR_STR_MAX);
	return 0;
}

SOCKET startupSocket()
{
	SOCKET sock;
	int optval = 1;

	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		fprintf(stderr, "Cannot open socket: %s ...!\n", strerror(errno));
		return -1;
	}
	
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval)) < 0)
	{
		fprintf(stderr, "Cannot set socket options: %s ...!\n", strerror(errno));
		return -1;
	}

	return sock;
}

int sendWOL(const wol_header_t* wol_header,SOCKET sock)
{
	struct sockaddr_in addr;
	unsigned char packet[PACKET_BUF];
	int i, j;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(REMOTE_PORT);
	addr.sin_addr.S_un.S_addr = inet_addr(wol_header->remote_addr);


	for (i = 0; i < 6; i++)
	{
		packet[i] = 0xFF;
	}

	for (i = 1; i <= 16; i++)
	{
		for (j = 0; j < 6; j++)
		{
			packet[i * 6 + j] = wol_header->mac_addr->mac_addr[j];
		}
	}

	if (sendto(sock,(const char*) packet, sizeof(packet), 0, (struct sockaddr*) & addr, sizeof(addr)) < 0)
	{
		fprintf(stderr, "Cannot send data: %s ...!\n", strerror(errno));
		return -1;
	}

	printf("Successful sent WOL magic packet to %s ...!\n", wol_header->mac_addr->mac_addr_str);
	return 0;
}



int main()
{
	

	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);
	mac_addr_t macadr;
	wol_header_t wolheader;

	wolheader.mac_addr = &macadr;

	memcpy(&wolheader.remote_addr,"192.168.178.255", 16);

	packMacAddr("70-8B-CD-58-63-40", &macadr);

	SOCKET sock = startupSocket();

	sendWOL(&wolheader, sock);

	WSACleanup();

	getchar();
	return 0;
}

