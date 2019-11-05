#ifndef WAKEONLAN_H
#define WAKEONLAM_H

#include "net.h"


typedef struct wol_paket
{
	net_byte data[108];
	int len;
}wol_paket;

class wakeonlansocket
{
public:
	wakeonlansocket();
	~wakeonlansocket();

	bool create(u_short port, const char* ip);
	bool createbroadcast(u_short port, const char* broadcastip);

	static bool loadpacket(wol_paket* paket, const char* macaddr, const char* pwd);
	bool send(wol_paket* paket);

private:
	SOCKET sock;
	sockaddr_in addr;
	bool sockinit;

	void reset();
};

#endif