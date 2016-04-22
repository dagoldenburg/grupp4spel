#ifndef PROCESSEVENTS_H_INCLUDED
#define PROCESSEVENTS_H_INCLUDED

int getmPosX(GameState *game);
int getmPosY(GameState *game);
int collisionDetection(Entity *object);
int processEvents(SDL_Window *window, GameState *game);

#endif // PROCESSEVENTS_H_INCLUDED
