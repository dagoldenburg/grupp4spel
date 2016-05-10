#include <SDL.h>
#include <SDL_net.h>
#include <SDL_thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#else
#include <windows.h>
#include <time.h>
#endif

#include "main.h"

#include "playField.h"
#include "gameStruct.h"
#include "gameNet.h"

#define GAME_PORT 1337
#define GAME_IP "127.0.0.1"
#define BUFFSIZE 4096

/*############################ LULZ ############################*/

void *recvfunc(void *sock)
{
	TCPsocket sock=(TCPsocket)Sock;
	int t;
	while (1) {
		if ((SDLNet_TCP_Recv(sock, recvbuffer, 100)) > 0) {
			recvbuffer[t] = '\0';
			//  sscanf(buffer"%d %d",)
			printf("> %s", recvbuffer);
		}
		else {
			if (t <= 0) perror("recv");
			else printf("Server closed connection\n");
			exit(1);
		}
	}
}

/* HEJ */
int TCP_socket_connection()
{
	IPaddress SERVER_IP;
	TCPsocket TCP_SOCKET;
	int counter = 0;
	int i, t;
	char *recvBuffer = malloc(100 * sizeof(char));

	// SDL INIT + Felhantering
	if (SDL_Init(0) < 0) {
		printf("SDL_Init(0): %s\n", SDL_GetError());
		exit(1);
	}

	// SDL_NET INIT + Felhantering
	if (SDLNet_Init() < 0) {
		printf("SDLNet_Init(): %s\n", SDLNet_GetError());
		SDL_Quit();
		exit(1);
	}


	/* choose a name and deal with it */
	char name[25];
	printf("Enter your name(max 25 letters): ");

	fgets(name, 100, stdin);

	for (i = 0; i<25; i++)
		if (name[i] == '\n')
			name[i] = '\0';

	while (1) {
		// Koppla till remote host
		printf("Connecting....\n");

		//SDLNet_ResolveHost(&SERVER_IP, GAME_IP, GAME_PORT);

		if (SDLNet_ResolveHost(&SERVER_IP, GAME_IP, GAME_PORT) == -1) {
			printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
			exit(1);
		}

		if (SERVER_IP.host == INADDR_NONE) {
			printf("SDLNet_ResolveHost: The address could not be resolved\n");
		}
		else {
			TCP_SOCKET = SDLNet_TCP_Open(&SERVER_IP);

			if (!TCP_SOCKET) {
				printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
				exit(2);
				if (TCP_SOCKET == NULL) {
					printf("Connect failed\n");
				}
				else {
					printf("Connected\n");
				}
			}
		}


		/* login with a name */
		int nameLen = strlen(name);
		if (SDLNet_TCP_Send(TCP_SOCKET, name, nameLen) < nameLen) {
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			SDLNet_TCP_Close(TCP_SOCKET);
			exit(1);
		}

		if ((t = SDLNet_TCP_Recv(TCP_SOCKET, recvBuffer, 100)) > 0) {
			recvBuffer[t] = '\0';
		}

		if (strstr(recvBuffer, "Server full\n")) {
			if (counter == 0) {
				counter++;
				SDLNet_TCP_Close(TCP_SOCKET);
			}
			else if (counter == 1) {
				printf("Server full, disconnecting.\n");
				SDLNet_TCP_Close(TCP_SOCKET);
				return 0;
			} 
		}
		else {
			break;
		}
	} // While

	// Ska göras om till SDL_NET
	/*
	pthread_create(&recvThread,NULL,recvfunc,(void *)&s); <----- Ska ändras

    	SDLNet_TCP_Close(TCP_SOCKET);
	return TCP_SOCKET;
	*/
}
