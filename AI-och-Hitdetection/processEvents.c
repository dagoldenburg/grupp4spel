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

int processEvents(SDL_Window *window, GameState *game,SDL_Rect camera, long ticks)
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
    ControlPlayer(game, ticks);
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

int hitAI(Entity *player, Entity *AI){
    if(spriteFacing.x == 32 && spriteFacing.y == 32){
        AI->rect.x -= 5;
        if(collisionDetection(AI)){
            AI->rect.x += 5;
        }
    }
    if(spriteFacing.x == 0 && spriteFacing.y == 32){
        AI->rect.x += 10;
        if(collisionDetection(AI)){
            AI->rect.x -= 5;
        }
    }
    if(spriteFacing.x == 0 && spriteFacing.y == 0){
        AI->rect.y -= 5;
        if(collisionDetection(AI)){
            AI->rect.y += 5;
        }

    }
    if(spriteFacing.x == 32 && spriteFacing.y == 0){
        AI->rect.y += 5;
        if(collisionDetection(AI)){
            AI->rect.y -= 5;
        }
    }

    resetAttack(player);
    if((AI->hpData.currentHp -= player->strength) <0) {
        return 1;
    }

    return 0;

}

void resetAttack(Entity *entity){
    entity->attack.h = 0;
    entity->attack.w = 0;
    entity->attack.x = 0;
    entity->attack.y = 0;
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
