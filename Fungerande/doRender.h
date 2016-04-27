#ifndef DORENDER_H_INCLUDED
#define DORENDER_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#include "gameStruct.h"
#include "playField.h"

void doRender(SDL_Renderer *renderer, GameState *game, SDL_Rect mCam);

#endif // DORENDER_H_INCLUDED
