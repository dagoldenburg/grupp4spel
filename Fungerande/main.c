#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "playField.h"
#include "gameStruct.h"
#include "loadMedia.h"
#include "processEvents.h"
#include "collision.h"
#include "doRender.h"
#include "createEntity.h"


    SDL_Rect spriteFacing;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    GameState gamestatePlayer;
    GameState gamestateAI[100];
    int nrofAi = 0;

    SDL_Window *window=NULL;                    // Declare a window
    SDL_Renderer *renderer=NULL;                // Declare a renderer

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
    //srandom((int)time(NULL));


  //Create an application window with the following settings:
    window = SDL_CreateWindow("Game Window",                     // window title
                            SDL_WINDOWPOS_UNDEFINED,           // initial x position
                            SDL_WINDOWPOS_UNDEFINED,           // initial y position
                            SCREEN_WIDTH,                               // width, in pixels
                            SCREEN_HEIGHT,                               // height, in pixels
                            0                                  // flags
                            );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    GameState tempEntity;

    SDL_Rect camera = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

    //Player attributes
    gamestatePlayer = createEntity(tempEntity,0,0,renderer);

    for(int i=0; i<rand()%20+5; i++)
    {
        gamestateAI[i]=createEntity(tempEntity, 1184, rand()%1000, renderer);
        gamestateAI[i].Entity.mVelY = rand()%2;
        nrofAi++;
    }

    loadMedia(&gamestatePlayer,&gamestateAI[0]);

  // The window is open: enter program loop (see SDL_PollEvent)


    /**Event loop*/
    int done = 0;
    spriteFacing.w = 32;
    spriteFacing.h = 32;
  //Event loop
    while(!done)
    {
    //Check for events
        for(int i=0; i<nrofAi; i++)
        {
            gamestateAI[i].XPOStmp=getAIPositionX(&gamestateAI[i]);
            gamestateAI[i].YPOStmp=getAIPositionY(&gamestateAI[i]);
            AITick(&gamestateAI[i]);
        }


        done = processEvents(window, &gamestatePlayer,camera);
        camera.x=(getmPosX(&gamestatePlayer)+ 20/2)-(SCREEN_WIDTH/2);
        camera.y=(getmPosY(&gamestatePlayer)+20/2)-(SCREEN_HEIGHT/2);
        if( camera.x < 0 ){
            camera.x = 0;
        }
        if( camera.y < 0 ){
            camera.y = 0;
        }
        if( camera.x > LEVEL_WIDTH - camera.w ){
            camera.x = LEVEL_WIDTH - camera.w;
        }
        if( camera.y > LEVEL_HEIGHT - camera.h ){
            camera.y = LEVEL_HEIGHT - camera.h;
        }

    //Render display
        doRender(renderer, &gamestatePlayer,camera,gamestateAI,nrofAi);

    //don't burn up the CPU
       SDL_Delay(10);

    }

  // Close and destroy the window
    SDL_DestroyTexture(gamestatePlayer.gTileTexture.mTexture);
    SDL_DestroyTexture(gamestatePlayer.gPlayerTexture.mTexture);
    SDL_DestroyTexture(gamestateAI->gPlayerTexture.mTexture);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();
    return 0;
}
