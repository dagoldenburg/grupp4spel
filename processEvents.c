#include "SDL.h"
//#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "playField.h"
#include "gameStruct.h"
#include "loadMedia.h"
#include "processEvents.h"
#include "player.h"
#include "collision.h"

int getmPosX(GameState *game)
{
    return game->Entity.rect.x;
}
int getmPosY(GameState *game)
{
    return game->Entity.rect.y;
}



int processEvents(SDL_Window *window, GameState *game,SDL_Rect camera)
{
    SDL_Event event;
    int done = 0;
    static int lastHpTick = 0;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
          case SDL_WINDOWEVENT_CLOSE:
          {
            if(window)
            {
              SDL_DestroyWindow(window);
              window = NULL;
              done = 1;
            }
          }
          break;
          case SDL_KEYDOWN:
          {
            switch(event.key.keysym.sym)
            {
              case SDLK_ESCAPE:
                done = 1;
              break;
            }
          }
          break;
          case SDL_QUIT:
            //quit out of the game
            done = 1;
          break;
        }
    }
    ControlPlayer(game);
    PlayerWallCollision(game);

    if(lastHpTick!= game->Entity.hpData.currentHp)
        UpdateHealthbar(game);
    lastHpTick = game->Entity.hpData.currentHp;

        game->Entity.hpData.healthBarMax.w = game->Entity.hpData.sizeOfHealthbar;
        game->Entity.hpData.healthBarMax.x = game->Entity.rect.x;
        game->Entity.hpData.healthBarMax.y = game->Entity.rect.y - 16;
        game->Entity.hpData.healthBarCurrent.x = game->Entity.rect.x;
        game->Entity.hpData.healthBarCurrent.y = game->Entity.rect.y - 16;


    //printf("manx: %d\n",game->Entity.mPosX );
    //printf("many: %d\n",game->Entity.mPosY );

  return done;

}
void AITick(GameState *Entity){

    GameState.Entity.rect.x--;
    return;
}
void whatSprite(GameState aiArray[])
{
    int ms = SDL_GetTicks();
    int sprite = ms /150 %3;
    for(int i=0; i<nrofAi; i++)
    {

                    if(getAIPositionX(aiArray[i])>aiArray[i].XPOStmp)
                       {
                           aiArray[i].Source.x = sprite*32;
                           aiArray[i].Source.y = 64;
                           aiArray[i].Source.w = 32;
                           aiArray[i].Source.h = 32;

                       }
                    else if(getAIPositionX(aiArray[i])<aiArray[i].XPOStmp)
                       {
                           aiArray[i].Source.x = sprite*32;
                           aiArray[i].Source.y = 32;
                           aiArray[i].Source.w = 32;
                           aiArray[i].Source.h = 32;
                       }
                    else if(getAIPositionY(aiArray[i])>aiArray[i].YPOStmp)
                       {
                           aiArray[i].Source.x = sprite*32;
                           aiArray[i].Source.y = 0;
                           aiArray[i].Source.w = 32;
                           aiArray[i].Source.h = 32;

                       }
                    else if(getAIPositionY(aiArray[i])<aiArray[i].YPOStmp])
                       {
                           aiArray[i].Source.x = sprite*32;
                           aiArray[i].Source.y = 96;
                           aiArray[i].Source.w = 32;
                           aiArray[i].Source.h = 32;
                       }

        }
}
