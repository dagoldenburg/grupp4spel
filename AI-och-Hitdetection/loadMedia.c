#include "SDL.h"
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "playField.h"
#include "gameStruct.h"
#include "loadMedia.h"
#include "processEvents.h"


void loadMedia(GameState *game, GameState *AI)
{
     SDL_Surface *loadedSurface=NULL;

    loadedSurface=IMG_Load("tiles.png");
    if(loadedSurface==NULL)
    {
        printf("%s This is the Tiles sprite\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    game->gTileTexture.mTexture=SDL_CreateTextureFromSurface(game->renderer,loadedSurface);

    game->TileClip[0].x=0;
    game->TileClip[0].y=0;
    game->TileClip[0].w=80;
    game->TileClip[0].h=80;

    game->TileClip[1].x=0;
    game->TileClip[1].y=80;
    game->TileClip[1].w=80;
    game->TileClip[1].h=80;

    game->TileClip[2].x=0;
    game->TileClip[2].y=160;
    game->TileClip[2].w=80;
    game->TileClip[2].h=80;

    game->TileClip[3].x=80;
    game->TileClip[3].y=0;
    game->TileClip[3].w=80;
    game->TileClip[3].h=80;



//    if(game->gTileTexture.mTexture==NULL)
//    {
//        printf( "Unable to create texture from game->gTileTexture.mTexture! SDL Error: \n");
//
//    }
//    else
//    {
////        game->gTileTexture.mWidth=loadedSurface->w;
////        game->gTileTexture.mHeight=loadedSurface->h;
//    }
    SDL_FreeSurface( loadedSurface );

    loadedSurface=IMG_Load("spriteSheet2.bmp");
    if(loadedSurface==NULL)
    {
        printf("%s This is the Player sprite\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xE0, 0x80, 0xC0));
    game->gPlayerTexture.mTexture=SDL_CreateTextureFromSurface(game->renderer,loadedSurface);
    if(game->gPlayerTexture.mTexture==NULL)
    {
        printf( "Unable to create texture from game->gDotTexture.mTexture! SDL Error: \n",SDL_GetError());

    }
    else
    {
        game->gPlayerTexture.mWidth=loadedSurface->w;
        game->gPlayerTexture.mHeight=loadedSurface->h;
    }
    SDL_FreeSurface( loadedSurface );


        loadedSurface=IMG_Load("Sprite.png");
    if(loadedSurface==NULL)
    {
        printf("%s This is the AI sprite\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0x0, 0xFF));
    AI->gPlayerTexture.mTexture=SDL_CreateTextureFromSurface(AI->renderer,loadedSurface);
    if(AI->gPlayerTexture.mTexture==NULL)
    {
        printf( "Unable to create texture from game->gDotTexture.mTexture! SDL Error: \n",SDL_GetError());

    }
    else
    {
        AI->gPlayerTexture.mWidth=loadedSurface->w;
        AI->gPlayerTexture.mHeight=loadedSurface->h;
    }
    SDL_FreeSurface( loadedSurface );

}
