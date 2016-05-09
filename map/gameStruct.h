#ifndef GAMESTRUCT_H_INCLUDED
#define GAMESTRUCT_H_INCLUDED
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
typedef struct
{
    int mWidth;
    int mHeight;
    SDL_Rect rect;
    SDL_Texture* mTexture;

}gameObject;

typedef struct {
    int sizeOfHealthbar;
    float maxHp;
    float currentHp;
    SDL_Rect healthBarCurrent;
    SDL_Rect healthBarMax;
} HitPointData;

typedef struct
{
    gameObject object;
    HitPointData hpData;
    SDL_Rect spriteFacing;
    int mPosX;
    int mPosY;
    int mVelX;
    int mVelY;
    int id;

}Entity;

typedef struct
{
    SDL_Rect TileClip[4]; ///MAP OBJECT
    gameObject gTileTexture;
    //////////////////////////////////
    Entity playerEntity[6];    /// PLAYER OBJECT

    Entity AiEntity[100];
    int nrOfAi;
    int socket;
    SDL_Renderer *renderer;

}GameState;

#endif /// GAMESTRUCT_H_INCLUDED
