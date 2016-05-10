

#include "gameGraphics.h"
#include "gameStruct.h"
#include "playField.h"
#include "gameNet.h"
#include "main.h"
void loadMedia(GameState *game)
{
    int i;
     SDL_Surface *loadedSurface=NULL;

        ///****************************MAP-Textures****************************///
    loadedSurface=IMG_Load("textures/tiles.png");
    if(loadedSurface==NULL)
    {
        printf("Cannot find tiles.png\n\n");
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

    SDL_FreeSurface( loadedSurface );
        ///***********************************player Texture********************************///

    loadedSurface=IMG_Load("textures/player1.png");
    if(loadedSurface==NULL)
    {
        printf("Cannot find tiles textures/player1.png\n\n");
        SDL_Quit();
        exit(1);

    }
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0x0, 0xFF));

    game->mPlayerTexture=SDL_CreateTextureFromSurface(game->renderer,loadedSurface);
    if(game->mPlayerTexture==NULL)
    {
        printf( "Unable to create texture from game->playerEntity.object.mTexture! SDL Error: \n");

    }
    SDL_FreeSurface( loadedSurface );

    ///*****************************************AI texture********************************///
    loadedSurface=IMG_Load("textures/player1.png");
    if(loadedSurface==NULL)
    {
        printf("Cannot find tiles textures/player1.png\n\n");
        SDL_Quit();
        exit(1);

    }
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0x0, 0xFF));
        game->mAiTexture=SDL_CreateTextureFromSurface(game->renderer,loadedSurface);
        if(game->mAiTexture==NULL)
        {
            printf( "Unable to create texture from game->AiEntity.object.mTexture! SDL Error: \n");

        }
    SDL_FreeSurface( loadedSurface );

}

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
        SDL_Rect tmp;
    //set the drawing color to white
   // SDL_Rect rect = {game->playerEntity.mPosX-mCam.x,game->playerEntity.mPosY-mCam.y, 32, 32 };
    for(int i=0;i<5;i++)
        SDL_RenderCopy(renderer,game->mPlayerTexture,&game->playerEntity[i].spriteFacing,getRenderPosition(game->playerEntity[i].object.rect,mCam,tmp));

//    SDL_Rect AIrect = {game->AiEntity.mPosX-mCam.x,game->AiEntity.mPosY-mCam.y, 32, 32 };
//    SDL_RenderCopy(renderer,game->AiEntity.object.mTexture,&game->AiEntity.SpriteFacing,&AIrect);
   // SDL_RenderFillRect(renderer, &rect);
   for(int i=0;i<game->nrOfAi;i++)
   {
        whatSprite(&game->AiEntity[i]);
        SDL_RenderCopy(renderer,game->mAiTexture,&game->AiEntity[i].spriteFacing,getRenderPosition(game->AiEntity[i].object.rect,mCam,tmp));
   }
        SDL_RenderPresent(renderer);

}
    SDL_Rect *getRenderPosition(SDL_Rect entity,SDL_Rect mCam,SDL_Rect temp)
    {
        temp.x=entity.x-mCam.x;
        temp.y=entity.y-mCam.y;
        temp.w=32;
        temp.h=32;
        return &temp;
    }
