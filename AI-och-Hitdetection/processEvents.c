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
void AITick(GameState *entity){

   /* if(entity->Entity.mVelY== 33 || entity->Entity.mVelY == -32){
        entity->Entity.mVelY = rand()%2;
    }
    */
    entity->Entity.rect.x--;
    if(collisionDetection(entity)){
        entity->Entity.rect.x++;

        if(entity->Entity.mVelY >= 1) {
            entity->Entity.rect.y--;
            if(collisionDetection(entity) || entity->Entity.rect.y <0){
                entity->Entity.rect.y++;
                entity->Entity.mVelY = 0;
            }
        }else if(entity->Entity.mVelY <=0) {
            entity->Entity.rect.y++;

            if(collisionDetection(entity) || entity->Entity.rect.y + TILESIZE + PLAYER_SPEED>LEVEL_HEIGHT){
                entity->Entity.rect.y--;
                entity->Entity.mVelY = 1;
            }
        }


    }


    PlayerWallCollision(entity);

    return;
}

void hitAI(Entity *player, Entity *AI){
    if(spriteFacing.x == 32 && spriteFacing.y == 32){
        AI->rect.x -= 5;
    }
    if(spriteFacing.x == 0 && spriteFacing.y == 32){
        AI->rect.x += 5;
    }
    if(spriteFacing.x == 0 && spriteFacing.y == 0){
        AI->rect.y -= 5;
    }
    if(spriteFacing.x == 32 && spriteFacing.y == 0){
        AI->rect.y += 5;
    }

    player->attack.h = 0;
    player->attack.w = 0;
    player->attack.x = 0;
    player->attack.y = 0;
}

void whatSprite(GameState *AI, int nrofAI)
{
    int ms = SDL_GetTicks();
    int sprite = ms /150 % 3+1;

                    if(getAIPositionX(&AI)>AI->XPOStmp)
                       {
                           AI->Source.x = sprite*32;
                           AI->Source.y = 64;
                           AI->Source.w = 32;
                           AI->Source.h = 32;

                       }
                    else if(getAIPositionX(&AI)<AI->XPOStmp)
                       {
                           AI->Source.x = sprite*32;
                           AI->Source.y = 32;
                           AI->Source.w = 32;
                           AI->Source.h = 32;
                       }
                    else if(getAIPositionY(&AI)>AI->YPOStmp)
                       {
                           AI->Source.x = sprite*32;
                           AI->Source.y = 0;
                           AI->Source.w = 32;
                           AI->Source.h = 32;

                       }
                    else if(getAIPositionY(&AI)<AI->YPOStmp)
                       {
                           AI->Source.x = sprite*32;
                           AI->Source.y = 96;
                           AI->Source.w = 32;
                           AI->Source.h = 32;
                       }


}
