#ifndef GAMESTRUCT_H_INCLUDED
#define GAMESTRUCT_H_INCLUDED
#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>


#elif __linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>

#endif
typedef struct // ett objekt
{
    SDL_Rect rect;
    SDL_Texture* mTexture;

}gameObject;

typedef struct { // information om hp
    int sizeOfHealthbar;
    float maxHp;
    float currentHp;
    SDL_Rect healthBarCurrent;
    SDL_Rect healthBarMax;
} HitPointData;

typedef struct { // informaton om attack
    SDL_Rect rect;
    int velX;
    int velY;
    SDL_Rect spriteFacing;
} Attack;

typedef struct // information om entities
{
    gameObject object;
    HitPointData hpData;
    SDL_Rect spriteFacing;
    int id;
    int strength;
    Attack attack;
    int attackTimer;
    int facing; //0 North, 1 East, 2 South, 3 West, 4 Standstill
    int characterType; // 0 For warrior, 1 for Mage
    int deathAnimation;
    char name[12];

}Entity;

typedef struct // information om text och chat relaterade saker
{
    int enterPressedFlag;
    SDL_Thread *chatThread;
    char chatMessage[30];
    char chatHistory[4][30];
    TTF_Font *font;
    SDL_Color color;
    SDL_Surface *textSurface;
    SDL_Texture *textTexture;
    int textW,textH,fontSize;
}Text;

typedef struct // information om tid
{
  Uint32 gameStartTime;
  Uint32 gameCurrentTime;
}Time;

typedef struct // spelets egen strukt som all nödvändig information ligger i
{
    SDL_Rect TileClip[4]; ///MAP OBJECT
    gameObject gTileTexture;
    SDL_Rect CastleTileClip[9];
    gameObject gCastleTexture;
    Text Text;
    Time Time;
    //////////////////////////////////
    Entity playerEntity[6];    /// PLAYER OBJECT
    Entity castle;
    Entity AiEntity[300];
    int aiEntityToken[300];
    int playerEntityToken [6];

    //diverse variabler, mest närrelaterade saker, kanske bör vara en egen net sub-struct
    int nrOfAi;
    TCPsocket socket;
    int mySlot;
    int playMode;
    int slotNeed;
    int rPlayers;
    int maxPlayers;
    SDL_Thread *recvThread;


    // grafik variabler, kanske bör ha en egen grafik sub struct
    SDL_Renderer *renderer;
    SDL_Texture* mAiTexture;
    SDL_Texture* mPlayerTexture;
    SDL_Texture* attackTexture;
    SDL_Texture* mageTexture;

}GameState;

#endif /// GAMESTRUCT_H_INCLUDED
