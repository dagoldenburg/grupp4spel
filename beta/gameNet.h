#ifndef GAMENET_H_INCLUDED
#define GAMENET_H_INCLUDED
#include <SDL2/SDL_net.h>

int TCP_socket_connection();
void *recvfunc(void *sock);
void safeSend(char sendString[],GameState *gamestate);

char recvbuffer[31];
extern int highestId;

#endif // GAMENET_H_INCLUDED
