#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "gameStruct.h"
#include "gameObject.h"
#include "main.h"
#include "gameGraphics.h"
#include "gameAction.h"
#include "gameAI.h"
#include "gameNet.h"
int main(int argc, char *argv[])
{
    pthread_t recvThread;
    srand(time(NULL));
    GameState gamestate;
    gamestate.socket=TCP_socket_connection();
    SDL_Window *window=NULL;                    // Declare a window
    SDL_Renderer *renderer=NULL;                // Declare a renderer
    Entity tempEntity;

    SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2
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
    gamestate.renderer=renderer;

    SDL_Rect cameraScene= {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

    gamestate.nrOfAi=0;
    loadMedia(&gamestate); ///load images to textures

  // The window is open: enter program loop (see SDL_PollEvent)

    /**Event loop*/

    pthread_create(&recvThread,NULL,recvfunc,(void *)&gamestate);
    int done = 0; // NEDANSTÅENDE SKA BLI EN FUNKTION SOM LIGGER I GAMENET
    char newPlayer[8],playerInfo[8];
    strcpy(newPlayer,"newplyr");
    strcpy(playerInfo,"plyrinf");
    safeSend(newPlayer,&gamestate);
    SDL_Delay(15); // två se s tätt inpå kan bli knas.
    safeSend(playerInfo,&gamestate);

    for(int i = 0;i<100;i++) // initializera ai token listan
        gamestate.aiEntityToken[i]=0;

  //Event loop
    while(!done) ///main game loop
    {
    //printf("myslot: %d", gamestate.mySlot);
    //Check for events
        for(int i=0;i<maxPlayers;i++){
            if(gamestate.playerEntityToken[i]==1){
                gamestate.playerEntity[i].mPosX=getAIPositionX(&gamestate.playerEntity[i]); /// get last mainplayers position
                gamestate.playerEntity[i].mPosY=getAIPositionY(&gamestate.playerEntity[i]);
                //coPlayerControll(&gamestate.playerEntity[i]);
            }
        }

        for(int i=0;i<highestId;i++)
        {
            gamestate.AiEntity[i].mPosX=getAIPositionX(&gamestate.AiEntity[i]); ///AI data
            gamestate.AiEntity[i].mPosY=getAIPositionY(&gamestate.AiEntity[i]);
            AITTick(&gamestate.AiEntity[i]); /// AI changes position and checks collision
            if(checkIFObjectHit(&gamestate.playerEntity[gamestate.mySlot].attack, &gamestate.AiEntity[i].object.rect)){//Kollar om spelarens attack kolliderar med AIn
                if(checkIFAiDead(&gamestate.playerEntity[gamestate.mySlot], &gamestate.AiEntity[i], &gamestate)){//Om kolliderat sänk hpen, om den har dött händer raderna nedan
                    aiDead(&gamestate, &gamestate.AiEntity[i]);
                }
                resetAttack(&gamestate.playerEntity[gamestate.mySlot].attack);
            }
        }

        done = processEvents(window, &gamestate);

        cameraScene.x=(getmPosX(&gamestate.playerEntity[gamestate.mySlot])+ 20/2)-SCREEN_WIDTH/2;
        cameraScene.y=(getmPosY(&gamestate.playerEntity[gamestate.mySlot])+20/2)-SCREEN_HEIGHT/2;
        if( cameraScene.x < 0 )   /// cameraScren follows main player
        {
            cameraScene.x = 0;
        }
        if( cameraScene.y < 0 )
        {
            cameraScene.y = 0;
        }
        if( cameraScene.x > LEVEL_WIDTH - cameraScene.w )
        {
            cameraScene.x = LEVEL_WIDTH - cameraScene.w;
        }
        if( cameraScene.y > LEVEL_HEIGHT - cameraScene.h )
        {
            cameraScene.y = LEVEL_HEIGHT - cameraScene.h;
        }


    //Render display
        doRender(renderer, &gamestate,cameraScene); /// renderer Ai , players and  map

    //don't burn up the CPU
        //SDL_Delay(10);

       ///***************if players position x,y changes -> send to server***********///
       if(getmPosX(&gamestate.playerEntity[gamestate.mySlot])!=gamestate.playerEntity[gamestate.mySlot].mPosX || getmPosY(&gamestate.playerEntity[gamestate.mySlot])!=gamestate.playerEntity[gamestate.mySlot].mPosY){
            printf("Attempting to update player movement\n");
            updatePlayerMovement(&gamestate);
        }
        }
  // Close and destroy the window
    SDL_DestroyTexture(gamestate.gTileTexture.mTexture);

    SDL_DestroyTexture(gamestate.mAiTexture);

    SDL_DestroyTexture(gamestate.playerEntity[0].object.mTexture); ///clear Textures
    SDL_DestroyTexture(gamestate.playerEntity[1].object.mTexture);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();
    //pthread_join(recvThread,NULL);
  //  close(gamestate.socket);

    return 0;
}
