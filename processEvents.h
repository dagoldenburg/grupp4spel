#ifndef PROCESSEVENTS_H_INCLUDED
#define PROCESSEVENTS_H_INCLUDED

int getmPosX(GameState *game);
int getmPosY(GameState *game);
int processEvents(SDL_Window *window, GameState *game,SDL_Rect camera);
void whatSprite(GameState aiArray[], int nrofAI);
void AITick(GameState *Entity);
#endif // PROCESSEVENTS_H_INCLUDED
