#include "wakeonlansocket.h"
#include <string.h>

typedef struct mac_addr_t
{
	unsigned char addr[6];
}mac_addr_t;

mac_addr_t StrToMac(const char* macaddr)
{
	mac_addr_t addr;
	char* str = (char*)malloc(strlen(macaddr) + 1);
	char* hp = NULL;
	strcpy(str, macaddr);
	memset(&addr, 0, sizeof(mac_addr_t));
	hp = strtok(str, "-: ");
	for (unsigned char adrpos = 0; adrpos < 6; adrpos++)
	{
		if (!hp) { break; }
		addr.addr[adrpos] = (unsigned char)strtol(hp, NULL, 16);
		hp = strtok(NULL, "-: ");
	}
	free(str);
	return addr;
}



wakeonlansocket::wakeonlansocket()
{
	sockinit = false;
	reset();
}

wakeonlansocket::~wakeonlansocket()
{
	reset();
}

bool wakeonlansocket::create(u_short port, const char* ip)
{
	bool state = false;

	if (ip)
	{
		reset();
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		if(sock < -1) { return state; }
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		inet_pton(AF_INET, ip, &addr.sin_addr);
		state = true;
		sockinit = true;
		return state;
	}
	return state;
}

bool wakeonlansocket::createbroadcast(u_short port, const char *broadcastip)
{
	int broadcast = 1;
	bool state = false;

	if (broadcastip)
	{
		reset();
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock < 0) { return state; }
		if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, 4) < 0) { return state; }
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		inet_pton(AF_INET,broadcastip, &addr.sin_addr);
		state = true;
		sockinit = true;
		return state;
	}
	return state;
}


bool wakeonlansocket::loadpacket(wol_paket* paket, const char* macaddr, const char* pwd)
{
	unsigned char pos = 0;
	mac_addr_t adr;
	bool state = false;
	if (paket)
	{
		for (pos = 0; pos < 6; pos++)
		{
			paket->data[pos] = 0xff;
		}
		if (macaddr)
		{
			adr = StrToMac(macaddr);
			for (unsigned int maccnt = 1; maccnt <= 16; maccnt++)
			{
				for (pos = 0; pos < 6; pos++)
				{
					paket->data[maccnt * 6 + pos] = adr.addr[pos];
				}
			}
			paket->len = 102;
			state = true;
			if (pwd)
			{

			}
		}

	}
	return state;
}

bool wakeonlansocket::send(wol_paket* paket)
{
	int net_size = 0;
	do
	{
		net_size = sendto(sock, (const char*)&paket->data[0],paket->len, 0,(const sockaddr*)&addr, sizeof(sockaddr));
		if (net_size < 0) { return false; }
	} while (net_size != paket->len);
	return true;
}

void wakeonlansocket::reset()
{
	memset(&addr, 0, sizeof(sockaddr_in));
	if (sockinit)
	{
		closesocket(sock);
		sock = 0;
		sockinit = false;
	}
}
