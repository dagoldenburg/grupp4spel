#ifndef UPDATEDMENU_H_INCLUDED
#define UPDATEDMENU_H_INCLUDED
#include "gameStruct.h"

int startMenu(SDL_Renderer* renderer,GameState *g);
void lossMenu(SDL_Renderer *renderer);
int lobbyMenu(SDL_Renderer *renderer, GameState *g);

//extern pthread_t recvThread;
extern SDL_Thread *recvThread;
#endif // UPDATEDMENU_H_INCLUDED

