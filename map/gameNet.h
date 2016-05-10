#ifndef GAMENET_H_INCLUDED
#define GAMENET_H_INCLUDED

int TCP_socket_connection();
void *recvfunc(void *sock);

extern int highestId;

#endif // GAMENET_H_INCLUDED
