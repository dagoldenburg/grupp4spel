#include "SDL.h"
//#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "playField.h"
#include "gameStruct.h"
#include "doRender.h"

void doRender(SDL_Renderer *renderer, GameState *game,SDL_Rect mCam)
{
  //set the drawing color to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect temp = {0,0,0,0};

    //Clear the screen (to blue)
    SDL_RenderClear(renderer);
    for(int i=0;i<COLUMNS;i++)
    {
        for(int j=0;j<ROWS;j++)
        {
            if(playfield[i][j]==0)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[0],&camera2);
            }
             if(playfield[i][j]==1)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[1],&camera2);
            }
            if(playfield[i][j]==2)
            {
                SDL_Rect camera3={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[2],&camera3);
            }
            if(playfield[i][j]==3)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[3],&camera2);
            }
        }

    }

//
//    SDL_Rect camera={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
//    SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&mCam,&camera);

    SDL_SetRenderDrawColor(renderer, 255,255, 255, 255);

    //set the drawing color to white
    //SDL_Rect rect = {game->Entity.rect.x-mCam.x,game->Entity.rect.y-mCam.y, TILESIZE, TILESIZE };

    SDL_RenderCopy(renderer,game->gPlayerTexture.mTexture,&spriteFacing,getRenderPositions(game->Entity.rect, mCam, temp));


   // SDL_RenderFillRect(renderer, &rect);

    if(SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF )){
        printf("Could not set render draw color for healthbar max. SDL_ERROR: %s\n", SDL_GetError());
    }

    if(SDL_RenderFillRect( renderer,getRenderPositions(game->Entity.hpData.healthBarMax, mCam, temp))<0){
        printf("Couldnt render fill rect healthbar max. SDL_ERROR: %s\n", SDL_GetError());
    }
    if(SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF )){
        printf("Could not set render draw color for healthbar current. SDL_ERROR: %s\n", SDL_GetError());
    }
    if(SDL_RenderFillRect( renderer, getRenderPositions(game->Entity.hpData.healthBarCurrent, mCam, temp))<0){
        printf("Couldnt render fill rect for healthbar current. SDL_ERROR: %s\n", SDL_GetError());
    }

    SDL_RenderPresent(renderer);
}

SDL_Rect *getRenderPositions(SDL_Rect entity, SDL_Rect mCam, SDL_Rect temp) {
    temp.x = entity.x -mCam.x;
    temp.y = entity.y -mCam.y;
    temp.w = entity.w;
    temp.h = entity.h;

    return &temp;
}
