#ifndef PROCESSEVENTS_H_INCLUDED
#define PROCESSEVENTS_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

#include "init.h" 
#include "playField.h"
#include "gameStruct.h"
#include "loadMedia.h"
#include "player.h"

int getmPosX(GameState *game);
int getmPosY(GameState *game);
int collisionDetection(Entity *object);
int processEvents(SDL_Window *window, GameState *game, SDL_Rect camera);

#endif // PROCESSEVENTS_H_INCLUDED