#ifndef DORENDER_H_INCLUDED
#define DORENDER_H_INCLUDED

void doRender(SDL_Renderer *renderer, GameState *game,SDL_Rect mCam, GameState AIarray[], int nrOfAi);
SDL_Rect *getRenderPositions(SDL_Rect entity, SDL_Rect mCam, SDL_Rect temp);
#endif // DORENDER_H_INCLUDED
