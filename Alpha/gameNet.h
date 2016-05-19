#ifndef GAMENET_H_INCLUDED
#define GAMENET_H_INCLUDED

int TCP_socket_connection();
void *recvfunc(void *sock);
void safeSend(char sendString[],GameState *gamestate);


char recvbuffer[31];
extern int highestId;
extern int maxPlayers;

#endif // GAMENET_H_INCLUDED
