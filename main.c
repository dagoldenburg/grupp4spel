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
    char tmp[100];
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

    gamestate.playerEntity[0]= createEntity(&tempEntity,0,0);/// init mainplayers data
    gamestate.playerEntity[1]= createEntity(&tempEntity,50,50);///init other co-players dat
    gamestate.nrOfAi=0;
   /* for(int i =0;i<rand()%20+5;i++)
    {
        gamestate.AiEntity[i]=createEntity(&tempEntity, 600,rand()%800);
        gamestate.AiEntity[i].mVelY=rand()%2;
        gamestate.AiEntity[i].id=i; /// test one AI change to ofnrofAI
        gamestate.nrOfAi++;
    }*/


    loadMedia(&gamestate); ///load images to textures

  // The window is open: enter program loop (see SDL_PollEvent)

    /**Event loop*/
    int done = 0;

   for(int i = 0;i<100;i++) // initializera ai token listan
        gamestate.aiEntityToken[i]=0;

    pthread_create(&recvThread,NULL,recvfunc,(void *)&gamestate);
  //Event loop
    while(!done) ///main game loop
    {
    //Check for events
        gamestate.playerEntity[0].mPosX=getAIPositionX(&gamestate.playerEntity[0]); /// get last mainplayers position
        gamestate.playerEntity[0].mPosY=getAIPositionY(&gamestate.playerEntity[0]);

        gamestate.playerEntity[1].mPosX=getAIPositionX(&gamestate.playerEntity[1]); /// get laat coplayers position
        gamestate.playerEntity[1].mPosY=getAIPositionY(&gamestate.playerEntity[1]);

        for(int i=0;i<highestId;i++)
        {
            gamestate.AiEntity[i].mPosX=getAIPositionX(&gamestate.AiEntity[i]); ///AI data
            gamestate.AiEntity[i].mPosY=getAIPositionY(&gamestate.AiEntity[i]);
            AITTick(&gamestate.AiEntity[i]); /// AI changes position and checks collision
        }

        done = processEvents(window, &gamestate);

        cameraScene.x=(getmPosX(&gamestate.playerEntity[0])+ 20/2)-SCREEN_WIDTH/2;
        cameraScene.y=(getmPosY(&gamestate.playerEntity[0])+20/2)-SCREEN_HEIGHT/2;
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
        if(getmPosX(&gamestate.playerEntity[0])!=gamestate.playerEntity[0].mPosX || getmPosY(&gamestate.playerEntity[0])!=gamestate.playerEntity[0].mPosY)
        {
            sprintf(tmp,"%d %d",getmPosX(&gamestate.playerEntity[0]),getmPosY(&gamestate.playerEntity[0]));
            if (send(gamestate.socket, tmp, strlen(tmp), 0) == -1)
            {
                perror("send");
                exit(1);
            }
        }
       // printf("from server %s\n",recvbuffer);///recv thread from other clients
    }
  // Close and destroy the window
    SDL_DestroyTexture(gamestate.gTileTexture.mTexture);

    SDL_DestroyTexture(gamestate.AiEntity[0].object.mTexture);

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
