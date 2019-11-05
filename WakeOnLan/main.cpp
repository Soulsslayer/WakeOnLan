#include <stdio.h>
#include "net.h"
#include "wakeonlansocket.h"


int main()
{
	wakeonlansocket sock;
	wol_paket packet;
	
	if (!net_init())
	{
		printf("Fehler beim Initialisieren des Netzwerkes");
	}

    if (!sock.createbroadcast(7, "192.168.178.255"))
	{
		printf("Fehler beim Erstellen des Sockets");
	}
   
	
	if(!sock.loadpacket(&packet, "70-8B-CD-58-63-40", NULL))
	{
		printf("ERROR beim Laden der Packete");
	}


	if (!sock.send(&packet))
	{
		printf("Fehler beim versenden de WOL packets");
	}
	else
	{
		printf("Konnte Erfolgreich WOL packet schicken");
	}

	net_quit();
	getchar();
	return 0;
}