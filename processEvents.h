#ifndef PROCESSEVENTS_H_INCLUDED
#define PROCESSEVENTS_H_INCLUDED

int getmPosX(GameState *game);
int getmPosY(GameState *game);
int processEvents(SDL_Window *window, GameState *game,SDL_Rect camera);

#endif // PROCESSEVENTS_H_INCLUDED
