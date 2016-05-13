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
    SDL_Rect CastleTileClip[9];
    gameObject gCastleTexture;
    //////////////////////////////////
    Entity playerEntity[6];    /// PLAYER OBJECT
    Entity castle;
    Entity AiEntity[100];
    int aiEntityToken[100];
    int playerEntityToken [6];

    int nrOfAi;
    int socket;
    int mySlot;
    SDL_Renderer *renderer;
    SDL_Texture* mAiTexture;
    SDL_Texture* mPlayerTexture;

}GameState;

#endif /// GAMESTRUCT_H_INCLUDED
