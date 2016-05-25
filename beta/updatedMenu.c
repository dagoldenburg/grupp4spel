#include "gameGraphics.h"
#include "gameStruct.h"
#include "playField.h"
#include "gameNet.h"
#include "main.h"
#include <SDL2/SDL_ttf.h>
#include "pthread.h"

pthread_t recvThread;

int lobbyMenu(SDL_Renderer *renderer, GameState *g){
    int mX, mY,textW,textH,players=1,i;
    int quit=0;
    char ready[30];
    int readyToggle=0;
    char connectedPlayers[30]="Connected players: /6 ";
    char readyPlayers[30]="Ready players: /3";
    //SNPRINTF CONNECTED PALYERS
    TTF_Init();
   TTF_Font * font = TTF_OpenFont("textures/LOUDNOISEBLACK.ttf", 20);
   SDL_Color color = {0,0,0};

   SDL_Surface *connectedPlayersSurface = TTF_RenderText_Solid(font,connectedPlayers,color);
   SDL_Texture *connectedPlayersTexture = SDL_CreateTextureFromSurface(renderer,connectedPlayersSurface);
   SDL_QueryTexture(connectedPlayersTexture,NULL,NULL,&textW,&textH);
   SDL_Rect connectedPlayersRect = { SCREEN_WIDTH/10,SCREEN_HEIGHT/6,textW,textH};

   SDL_Surface *readyPlayersSurface = TTF_RenderText_Solid(font,readyPlayers,color);
   SDL_Texture *readyPlayersTexture = SDL_CreateTextureFromSurface(renderer,readyPlayersSurface);
   SDL_QueryTexture(readyPlayersTexture,NULL,NULL,&textW,&textH);
   SDL_Rect readyPlayersRect = { SCREEN_WIDTH/10,SCREEN_HEIGHT/4,textW,textH};

    printf("player ready: %d\nMyslot: %d\n",readyToggle, g->mySlot);
    SDL_Event event;
    SDL_Surface *lobbyMenuSurface = IMG_Load("textures/LobbyReady.png");
    if(lobbyMenuSurface==NULL){
        printf("Loss menu surface failed to load. SDL_ERROR: %s\n",SDL_GetError());
    }
    SDL_Texture *lobbyMenuTexture = SDL_CreateTextureFromSurface(renderer,lobbyMenuSurface);
    if(lobbyMenuTexture==NULL){
        printf("Loss menu texture failed to load. SDL_ERROR: %s\n",SDL_GetError());
    }
    while(!quit){

    for(i=0;i<g->maxPlayers;i++){
        if(g->playerEntityToken[i]!=0){
            ++players;
        }
    }
        stringManipulate(players,18,connectedPlayers);
        stringManipulate(g->rPlayers,14,readyPlayers);
        players = 0;
       connectedPlayersSurface = TTF_RenderText_Solid(font,connectedPlayers,color);
       connectedPlayersTexture = SDL_CreateTextureFromSurface(renderer,connectedPlayersSurface);
       readyPlayersSurface = TTF_RenderText_Solid(font,readyPlayers,color);
       readyPlayersTexture = SDL_CreateTextureFromSurface(renderer,readyPlayersSurface);

       while(SDL_PollEvent(&event)){

            //SDL_PumpEvents();
            SDL_GetMouseState(&mX,&mY);
            if(event.type == SDL_QUIT)
            {
              SDL_Quit();
              exit(1);
            }
            if(event.type == SDL_MOUSEBUTTONDOWN && (mX>SCREEN_WIDTH*0.566 && mX<SCREEN_WIDTH*0.9)) //READY
            {
                if(event.button.button == SDL_BUTTON_LEFT && (mY>= SCREEN_HEIGHT*0.775 && mY<=SCREEN_HEIGHT*0.945))
                {
                    if(readyToggle == 0){
                        lobbyMenuSurface = IMG_Load("textures/LobbyNotReady.png");
                        if(lobbyMenuSurface==NULL){
                            printf("Loss menu surface failed to load. SDL_ERROR: %s\n",SDL_GetError());
                        }
                        lobbyMenuTexture = SDL_CreateTextureFromSurface(renderer,lobbyMenuSurface);
                        strcpy(ready,"plrrdy!, ID:000................");
                        stringManipulate(g->mySlot,14,ready);
                        readyToggle = 1;
                    }
                    else if(readyToggle == 1){
                        lobbyMenuSurface = IMG_Load("textures/LobbyReady.png");
                        if(lobbyMenuSurface==NULL){
                            printf("Loss menu surface failed to load. SDL_ERROR: %s\n",SDL_GetError());
                        }
                        lobbyMenuTexture = SDL_CreateTextureFromSurface(renderer,lobbyMenuSurface);
                        if(lobbyMenuTexture==NULL){
                            printf("Loss menu texture failed to load. SDL_ERROR: %s\n",SDL_GetError());
                        }
                        strcpy(ready,"plrnrdy, ID:000................");
                        stringManipulate(g->mySlot,14,ready);
                        readyToggle = 0;
                      }
                        safeSend(ready,g);
                    }
                    // TODO: skriva you are ready med ttf
                }
               if(event.type == SDL_MOUSEBUTTONDOWN && (mX>SCREEN_WIDTH*0.0675 && mX<SCREEN_WIDTH*0.411)) // MAINMENU
               {
                    if(event.button.button == SDL_BUTTON_LEFT && (mY>=SCREEN_HEIGHT*0.77 && mY<=SCREEN_HEIGHT*0.94))
                    {
                        printf("mein menu pressed\n");
                        return 2;
                    }
               }
        }

        if(g->playMode==1){
            return 1;
        }

        SDL_RenderCopy(renderer,lobbyMenuTexture,NULL,NULL);
        SDL_RenderCopy(renderer,connectedPlayersTexture,NULL,&connectedPlayersRect);
        SDL_RenderCopy(renderer,readyPlayersTexture,NULL,&readyPlayersRect);
        SDL_RenderPresent(renderer);
    }
    return;
}

void lossMenu(SDL_Renderer *renderer){
    int mX, mY,quit=0;
    SDL_Rect menuRect;
    menuRect.w = 400;
    menuRect.h = 200;
    menuRect.x = (SCREEN_WIDTH/4);
    menuRect.y = (SCREEN_HEIGHT/3);
    SDL_Event event;
    SDL_Surface *lossMenuSurface = IMG_Load("textures/lossMenu.png");
    if(lossMenuSurface==NULL){
        printf("Loss menu surface failed to load. SDL_ERROR: %s\n",SDL_GetError());
    }
    SDL_Texture *lossMenuTexture = SDL_CreateTextureFromSurface(renderer,lossMenuSurface);
    if(lossMenuTexture==NULL){
        printf("Loss menu texture failed to load. SDL_ERROR: %s\n",SDL_GetError());
    }
    while(!quit){
        while(SDL_PollEvent(&event)){
            //SDL_PumpEvents();
            SDL_GetMouseState(&mX,&mY);
            if( event.type == SDL_QUIT )
            {
              SDL_Quit();
              exit(1);
            }
           if(event.type == SDL_MOUSEBUTTONDOWN && (mX>menuRect.x*1.645 && mX<menuRect.x*2.395)) //EXIT
            {
                if(event.button.button == SDL_BUTTON_LEFT && (mY>= menuRect.y*1.718 && mY<=menuRect.y*1.86))
                {
                printf("gracefull exit\n");
                    SDL_Quit();
                    exit(1);
                }
            }
           if(event.type == SDL_MOUSEBUTTONDOWN && (mX>menuRect.x*1.065 && mX<menuRect.x*2.965)) // MAINMENU
           {
                if(event.button.button == SDL_BUTTON_LEFT && (mY>=menuRect.y*1.46 && mY<=menuRect.y*1.615))
                {
                    printf("main menu pressed\n");
                    quit=1;
                }
           }
        }
        SDL_RenderCopy(renderer,lossMenuTexture,NULL,&menuRect);
        SDL_RenderPresent(renderer);
    }
}

int startMenu(SDL_Renderer* renderer,GameState *g)
{
       int whichError=0;
       int play = 0;
       int mX,mY,textW,textH,quit=0,players;
       char info[30]="IP:127.0.0.1 players: ";
       char player[10];
       snprintf(player, 30, "%d",players);
       strcat(info,player);
       char cantConnect[25]="Can't Connect to server";
       char serverFull[25]="Server full";
       SDL_Surface *loadedSurface = IMG_Load("textures/PixelMenu2.png");
       SDL_Surface *howToSurface = IMG_Load("textures/HowTo3.png");
       SDL_Event event;
       TTF_Init();
       TTF_Font * font = TTF_OpenFont("textures/LOUDNOISEBLACK.ttf", 20);
       SDL_Color color = {255,0,0};
       SDL_Surface *serverFullSurface = TTF_RenderText_Solid(font,serverFull,color);
       SDL_Surface *cantConnectSurface = TTF_RenderText_Solid(font,cantConnect,color);
        if(loadedSurface==NULL || cantConnectSurface==NULL || howToSurface==NULL)
        {
            printf("%s\n", SDL_GetError());
            SDL_Quit();
            exit(1);
        }
        SDL_SetColorKey(howToSurface, SDL_TRUE, SDL_MapRGB(howToSurface->format, 0xff, 0x00, 0xff));
        SDL_Texture *menuTexture = SDL_CreateTextureFromSurface(renderer,loadedSurface);
        SDL_Texture *cantConnectTexture = SDL_CreateTextureFromSurface(renderer, cantConnectSurface);
        SDL_Texture *howToTexture = SDL_CreateTextureFromSurface(renderer,howToSurface);
        SDL_Texture *serverFullTexture = SDL_CreateTextureFromSurface(renderer,serverFullSurface);
        if(menuTexture == NULL || cantConnectTexture == NULL || howToTexture ==NULL)
        {
            printf("%s\n", SDL_GetError());
            SDL_Quit();
            exit(1);
        }
        SDL_Rect howToRect={0,SCREEN_HEIGHT/3,300,150};
        SDL_QueryTexture(cantConnectTexture,NULL,NULL,&textW,&textH);
        SDL_Rect cantConnectRect = { SCREEN_WIDTH/2-textW/2,SCREEN_HEIGHT/4,textW,textH };
        SDL_QueryTexture(serverFullTexture,NULL,NULL,&textW,&textH);
        SDL_Rect serverFullRect = { SCREEN_WIDTH/2-textW/2,SCREEN_HEIGHT/4,textW,textH };
        //SDL_RenderCopy(renderer,directionsTexture,NULL,NULL);

       if(SDL_RenderCopy(renderer,menuTexture,NULL,NULL)!=0)
       {
           printf("%s RenderCopy fail\n", SDL_GetError());
            SDL_Quit();
            exit(1);
       }
       while(!quit)
       {
            SDL_Delay(16);
            while(SDL_PollEvent(&event))
            {
                //SDL_PumpEvents();
                SDL_GetMouseState(&mX,&mY);

                if( event.type == SDL_QUIT )
                {
                  SDL_Quit();
                  exit(1);
                }
               if(event.type == SDL_MOUSEBUTTONDOWN && (mX>=SCREEN_WIDTH*0.44 && mX<=SCREEN_WIDTH*0.59))
                {
                    if(event.button.button == SDL_BUTTON_LEFT)
                    {
                        if(mY>=SCREEN_HEIGHT*0.375 && mY<= SCREEN_HEIGHT*0.4)
                        {
                                printf("play pressed\n");
                                char newPlayer[30],playerInfo[30];
                                strcpy(newPlayer,"newplyr........................");
                                strcpy(playerInfo,"plyrinf........................");
                                newPlayer[29] = '\n'; //newline
                                newPlayer[30] = '\0'; // null
                                playerInfo[29] = '\n'; //newline
                                playerInfo[30] = '\0'; // null
                                g->socket=TCP_socket_connection();
                                printf("%d",g->socket);
                                if(g->socket==-1){
                                    whichError=1;
                                    printf("Server down\n");
                                    break;
                                }
                               else if(g->socket==-2){
                                    whichError=2;
                                    printf("Server full\n");
                                    break;
                                }
                                pthread_create(&recvThread,NULL,recvfunc,(void *)g);
                                safeSend(newPlayer,g);
                                SDL_Delay(10);
                                safeSend(playerInfo,g);
                                g->slotNeed = 1;
                                play = lobbyMenu(renderer, g);
                                    SDL_FreeSurface(loadedSurface);
                                    SDL_FreeSurface(cantConnectSurface);
                                    SDL_FreeSurface(howToSurface);
                                    SDL_DestroyTexture(howToTexture);
                                    SDL_DestroyTexture(cantConnectTexture);
                                    SDL_DestroyTexture(menuTexture);
                                    TTF_CloseFont(font);
                                    TTF_Quit();
                                return play;

                        }
                        else if(mY>= SCREEN_HEIGHT*0.525 && mY<=SCREEN_HEIGHT*0.59)
                        {
                            SDL_Quit();
                            exit(1);
                        }
                    }

                }
               if(event.type == SDL_MOUSEBUTTONDOWN && (mX>SCREEN_WIDTH*0.296 && mX<SCREEN_WIDTH*0.679))
               {
                    if(event.button.button == SDL_BUTTON_LEFT)
                    {
                        if(mY>=SCREEN_HEIGHT*0.4625 && mY<=SCREEN_HEIGHT*0.52)  //370 && 400
                        {
                                SDL_RenderCopy(renderer,howToTexture,NULL,&howToRect);
                                SDL_RenderPresent(renderer);
                        }
                    }
               }
                if(whichError==1)
                    SDL_RenderCopy(renderer,cantConnectTexture,NULL,&cantConnectRect);
                else if(whichError==2)
                    SDL_RenderCopy(renderer,serverFullTexture,NULL,&cantConnectRect);
                SDL_RenderPresent(renderer);
            }
       }

            SDL_FreeSurface(loadedSurface);
            SDL_FreeSurface(cantConnectSurface);
            SDL_FreeSurface(howToSurface);
            SDL_DestroyTexture(howToTexture);
            SDL_DestroyTexture(cantConnectTexture);
            SDL_DestroyTexture(menuTexture);
            TTF_CloseFont(font);
            TTF_Quit();

return;
}
