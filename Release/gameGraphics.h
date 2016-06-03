#ifndef GAMEGRAPHICS_H_INCLUDED
#define GAMEGRAPHICS_H_INCLUDED

#include "gameStruct.h"
#include "gameAI.h"
void loadMedia(GameState *game);
void doRender(SDL_Renderer *renderer, GameState *game,SDL_Rect mCam);
SDL_Rect *getRenderPosition(SDL_Rect entity,SDL_Rect mCam,SDL_Rect temp);
void updatehealthbar(Entity *aiEntity);
void renderShape(SDL_Renderer *renderer, SDL_Rect rect, SDL_Rect mCam);
void deathAnimation(Entity *AI);

#endif // GAMEGRAPHICS_H_INCLUDED
