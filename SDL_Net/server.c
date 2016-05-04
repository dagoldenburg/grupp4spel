// Under construction
//---
//---

/*
gcc server.c -o server -L./ -lSDL2main -lSDL2 -lSDL2_net
*/
//--------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#define MAX_SOCKET 1	// Hur många sockets ska man hålla koll på (för socket_set)

//--------------------------------------------------------
// Deklarerar server_socket och socket_set
UDPsocket server_socket;	// oklar UDP socket datatyp (pekare) Fildeskriptor                                                         
SDLNet_SocketSet socket_set;	// Funktion för multiplesockets - veta när en socket har kontakt/data


int main(int argc, char *argv[]) 
{
	int socketset_num;
	
	// Öppnar en socket fildeskriptor för send/recv + Felhantering
	server_socket = SDLNet_UDP_Open(1337);		// Success-> UDP Socket, Failure-> NULL
	
	if(!server_socket) {
    printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
    exit(2);
	}
	
	// Implementera socket set
	socket_set = SDLNet_AllocSocketSet(MAX_SOCKET);
	if(!socket_set) {
    printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
    exit(2);
	}
	
	socketset_num = SDLNet_UDP_AddSocket(socket_set, server_socket);
	if(socketset_num == -1) {
    printf("SDLNet_AddSocket: %s\n", SDLNet_GetError());
    // socketset_num = -1 => error. Enligt API:n kan det bero på att man behöver fler sockets.
	}
	
	for(;;) {
		
		// Checka socket set. Timeout=0 -> Betyder "no waiting"
		int socketset_rdy = SDLNet_CheckSockets(socket_set, 0);
		if(socketset_rdy == -1) {
			printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
		} if(!socketset_rdy) {
			// Om ingen aktivitet finns i socket_set så kanske man kan köra några underhållningsrutiner eller något.
		}
		
		// Checka socket för aktivitet. Aktiv= !0 - Inaktiv= 0
		// Har servern ett paket som väntar? 
		if(SDLNet_SocketReady(server_socket)) {
			UDPpacket packet;	// Skapa ett packet för client meddelande
			
			// Allokera minne för paketet -> 255 bytes
			packet.maxlen = 0xFF;
			packet.data = (uint8_t *)malloc(0xFF);		// uint8_t -> Samma sak som en byte. (a type of unsigned integer of length 8 bits)
			
			// Ta emot paketet (om det går/finns ett paket). Packet mottaget= 1, Inget packet= 0, Error= -1
			int check_recv = SDLNet_UDP_Recv(server_socket, &packet);
			if(!check_recv) {	// Om det är inget paket, släpp paketet.
				free(packet.data);
				continue;
			}
			
			printf("%s\n", packet.data);
			free(packet.data);
			
		}
	}
	// BeFrIa socket_set OcH server_socket(fIlDeSkRiPtOr)
	SDLNet_FreeSocketSet(socket_set);
	SDLNet_UDP_Close(server_socket);
	
	return 0;
}

  
