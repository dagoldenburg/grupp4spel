#include "gameGraphics.h"
#include "gameStruct.h"
#include "playField.h"
#include "gameNet.h"
#include "main.h"
#include <SDL2/SDL_ttf.h>
#include "pthread.h"

pthread_t recvThread;

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
    /*printf("mouseposition, x:%d y:%d ",mX,mY);
    printf("menurect, x:%d y:%d\n",menuRect.x,menuRect.y);
    printf("mainmenurect, start x:%f stop x: %f start y:%f stop y:%f\n",menuRect.x*1.065,menuRect.x*2.965,menuRect.y*1.46,menuRect.y*1.615);
    printf("exitrect, start x:%f stop x: %f start y:%f stop y:%f\n",menuRect.x*1.645,menuRect.x*2.395,menuRect.y*1.718,menuRect.y*1.86);*/
        while(SDL_PollEvent(&event)){
            SDL_PumpEvents();
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
                    quit=1;
                }
           }
        }
        SDL_RenderCopy(renderer,lossMenuTexture,NULL,&menuRect);
        SDL_RenderPresent(renderer);
    }
}

void startMenu(SDL_Renderer* renderer,GameState *g)
{
   int mX,mY,textW,textH,quit=0,players;
   char info[30]="IP:127.0.0.1 players: ";
   char player[10];
   snprintf(player, 30, "%d",players);
   strcat(info,player);
   char directions[50]="Press enter to view server and player count";
   SDL_Surface *loadedSurface = IMG_Load("textures/PixelMenu2.png");
   SDL_Surface *howToSurface = IMG_Load("textures/HowTo3.png");
   SDL_Event event;
   TTF_Init();
   TTF_Font * font = TTF_OpenFont("textures/LOUDNOISEBLACK.ttf", 20);
   SDL_Color color = {255,255,255};
   SDL_Surface * TextSurface = TTF_RenderText_Solid(font,info,color);
   SDL_Surface *InfoSurface = TTF_RenderText_Solid(font,directions,color);

    if(loadedSurface==NULL || TextSurface==NULL || InfoSurface==NULL || howToSurface==NULL)
    {
        printf("%s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    SDL_SetColorKey(howToSurface, SDL_TRUE, SDL_MapRGB(howToSurface->format, 0xff, 0x00, 0xff));
    SDL_Texture *menuTexture = SDL_CreateTextureFromSurface(renderer,loadedSurface);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer,TextSurface);
    SDL_Texture *directionsTexture = SDL_CreateTextureFromSurface(renderer, InfoSurface);
    SDL_Texture *howToTexture = SDL_CreateTextureFromSurface(renderer,howToSurface);
    if(menuTexture == NULL || textTexture == NULL || InfoSurface == NULL || howToTexture ==NULL)
    {
        printf("%s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    SDL_QueryTexture(textTexture,NULL,NULL,&textW,&textH);
    SDL_Rect dstRect = {800, 300, textW, textH};
    SDL_Rect howToRect={0,SCREEN_HEIGHT/3,400,200};
    //SDL_RenderCopy(renderer,directionsTexture,NULL,NULL);

   if(SDL_RenderCopy(renderer,menuTexture,NULL,NULL)!=0)
   {
       printf("%s RenderCopy fail\n", SDL_GetError());
        SDL_Quit();
        exit(1);

   }
   while(!quit)
   {
        while(SDL_PollEvent(&event))
        {
            SDL_PumpEvents();
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
                        char newPlayer[8],playerInfo[8];
                        strcpy(newPlayer,"newplyr");
                        strcpy(playerInfo,"plyrinf");
                        g->socket=TCP_socket_connection();
                        pthread_create(&recvThread,NULL,recvfunc,(void *)g);
                        safeSend(newPlayer,g);
                        SDL_Delay(15); // två se s tätt inpå kan bli knas. byt t till ngt vettigt TODO
                        safeSend(playerInfo,g);
                        quit=1;
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
        }
        SDL_RenderPresent(renderer);
   }

        SDL_FreeSurface(loadedSurface);
        SDL_FreeSurface(TextSurface);
        SDL_FreeSurface(InfoSurface);
        SDL_FreeSurface(howToSurface);
        SDL_DestroyTexture(howToTexture);
        SDL_DestroyTexture(directionsTexture);
        SDL_DestroyTexture(menuTexture);
        SDL_DestroyTexture(textTexture);
        TTF_CloseFont(font);
        TTF_Quit();
return;
}
