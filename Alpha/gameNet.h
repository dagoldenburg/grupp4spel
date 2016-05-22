#ifndef GAMENET_H_INCLUDED
#define GAMENET_H_INCLUDED
#include <SDL2/SDL_net.h>
#include "gameStruct.h"
TCPsocket TCP_socket_connection();
void *recvfunc(void *sock);
void safeSend(char sendString[],GameState *gamestate);

void stringManipulate(int intToString,int posInList,char* string);

char recvbuffer[31];
extern int highestId;
extern int maxPlayers;

#endif // GAMENET_H_INCLUDED
