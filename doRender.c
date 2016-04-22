#include "SDL.h"
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "playField.h"
#include "gameStruct.h"
#include "loadMedia.h"
#include "processEvents.h"

void doRender(SDL_Renderer *renderer, GameState *game,SDL_Rect mCam)
{
  //set the drawing color to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

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
    SDL_Rect rect = {game->Entity.mPosX-mCam.x,game->Entity.mPosY-mCam.y, 32, 32 };
    SDL_RenderCopy(renderer,game->gDotTexture.mTexture,NULL,&rect);
   // SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
}
