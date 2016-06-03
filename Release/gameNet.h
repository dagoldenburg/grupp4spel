#ifndef GAMENET_H_INCLUDED
#define GAMENET_H_INCLUDED
#ifdef _WIN32

#include <SDL_net.h>


#elif __linux
#include <SDL2/SDL_net.h>

#endif
TCPsocket TCP_socket_connection();
void chatFunc(void *gamestate);
void recvfunc(void *sock);
void safeSend(char sendString[],GameState *gamestate);
void stringManipulate(int intToString,int posInList,char* string);

char recvbuffer[31];
extern int highestId;

#endif // GAMENET_H_INCLUDED
