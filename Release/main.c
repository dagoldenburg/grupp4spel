
#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>


#elif __linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#endif
#include <pthread.h>

#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "gameStruct.h"
#include "gameObject.h"
#include "main.h"
#include "gameAI.h"
#include "gameAction.h"
#include "gameNet.h"
#include "updatedMenu.h"
#include "gameGraphics.h"
int threadJoinQuerry;

int main(int argc, char *argv[])
{
//    pthread_t recvThread;
    TTF_Init();
    srand(time(NULL));
    GameState gamestate;
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

  // The window is open: enter program loop (see SDL_PollEvent)

    /**Event loop*/

    int done = 0; // NEDANSTÅENDE SKA BLI EN FUNKTION SOM LIGGER I GAMENET
    int ticks = 0;

 // TODO GÖR 96x96

  gamestate.maxPlayers=6;
  //Event loop
  gamestate.playMode = 0;
  int play = 0;
  gamestate.slotNeed=0;
  gamestate.rPlayers = 0;
  gamestate.Text.enterPressedFlag = 0;
  gamestate.Text.chatMessage[0]='*';
  gamestate.Text.fontSize=20;

gamestate.Text.font = TTF_OpenFont("textures/LOUDNOISEBLACK.ttf", gamestate.Text.fontSize);
gamestate.Text.color.r = 255;
gamestate.Text.color.g = 255;
gamestate.Text.color.b = 255;
gamestate.Text.color.a = 255;
  while(1){
  for(int i = 0;i<300;i++) // initializera ai token listan
        gamestate.aiEntityToken[i]=0;
  for(int i = 0;i<gamestate.maxPlayers;i++)
    gamestate.playerEntityToken[i]=0;

    while(!play){
        play = startMenu(renderer,&gamestate);
    }
    if(play == 2){
        done =3;

    }
    else{
        gamestate.castle=createEntity(&tempEntity,896,508,96,96,1000.0);
        loadMedia(&gamestate); ///load images to textures
        gamestate.Time.gameStartTime = SDL_GetTicks();
    }
    while(!done) ///main game loop
    {
        if(ticks == 0 || SDL_GetTicks()-ticks > 32){
        for(int i=0;i<gamestate.maxPlayers;i++){
            if(gamestate.playerEntityToken[i]!=0){
                attackTick(&gamestate.playerEntity[i].attack);
            }
        }

        if(attackHandle(&gamestate.playerEntity[gamestate.mySlot].attack)) {
            resetAttack(&gamestate.playerEntity[gamestate.mySlot].attack);
            sendResetAttack(&gamestate);
            }
        for(int i=0;i<=highestId;i++){
            if(gamestate.aiEntityToken[i]!=0){
                if(gamestate.AiEntity[i].hpData.currentHp <= 0){
                    gamestate.aiEntityToken[i] = 0;
                    gamestate.AiEntity[i].deathAnimation = 1;
                }
            }
        }
        for(int i=0;i<=highestId;i++){
            //AITTick(&gamestate.AiEntity[i]); /// AI changes position and checks collision
            if(gamestate.aiEntityToken[i]!=0){
                if(checkIFObjectHit(&gamestate.playerEntity[gamestate.mySlot].attack, &gamestate.AiEntity[i].object.rect)){//Kollar om spelarens attack kolliderar med AIn
                    giveDamage(&gamestate,i);
                    if(gamestate.playerEntity[gamestate.mySlot].attack.velX !=0 || gamestate.playerEntity[gamestate.mySlot].attack.velY !=0) {
                        resetAttack(&gamestate.playerEntity[gamestate.mySlot].attack);
                        sendResetAttack(&gamestate);
                    }
                }
            }
        }
        if(gamestate.playerEntity[gamestate.mySlot].attack.velX == 0 && gamestate.playerEntity[gamestate.mySlot].attack.velY == 0 &&
        gamestate.playerEntity[gamestate.mySlot].attack.rect.h !=0 && gamestate.playerEntity[gamestate.mySlot].attack.rect.w != 0) {
            resetAttack(&gamestate.playerEntity[gamestate.mySlot].attack);
            sendResetAttack(&gamestate);
        }


        done = processEvents(window, &gamestate);
        //sendName(&gamestate);

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

       ///***************if players position x,y changes -> send to server***********///
       if(gamestate.playerEntity[gamestate.mySlot].facing !=4){
            updatePlayerMovement(&gamestate);
        }
        ///*********WIN CONDITION*******///
        gamestate.Time.gameCurrentTime = SDL_GetTicks();
        if(gamestate.Time.gameCurrentTime>gamestate.Time.gameStartTime+600000){
            done = 2;
        }
        ///*******'LOSS CONDITION*****////
        if(gamestate.castle.hpData.currentHp<=0){
            done=1;
        }
        ticks = SDL_GetTicks();
        }else{
        SDL_Delay(1);
        }
    }
    //TODO Rensa upp alla textures osv
    // kill thread
    gamestate.castle.hpData.currentHp=1000.0;
    char disconnect[30];
    strcpy(disconnect,"disconnecting...............");
    safeSend(disconnect,&gamestate);
    if(done==1)
        lossMenu(renderer);
    else if(done==2)
        lossMenu(renderer);
    gamestate.slotNeed = 0;
    play = 0;
    gamestate.playMode = 0;
    done = 0;
    gamestate.rPlayers = 0;
    printf("going around\n");
}
  // Close and destroy the window
    SDL_DestroyTexture(gamestate.gTileTexture.mTexture);

    SDL_DestroyTexture(gamestate.mAiTexture);

    SDL_DestroyTexture(gamestate.mPlayerTexture);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();
    //pthread_join(recvThread,NULL);
  //  close(gamestate.socket);

    return 0;
}
