#include "main.h"
#include "playField.h"
#include "gameObject.h"
#include "gameAction.h"
#include "time.h"
#include "gameAI.h"

Entity createEntity(Entity *entity,int x ,int y)
{
    entity->mVelX=0;
    entity->mVelY=0;
    entity->spriteFacing.x=0;
    entity->spriteFacing.y=0;
    entity->spriteFacing.w=32;
    entity->spriteFacing.h=32;
    entity->mPosX=0;
    entity->mPosY=0;
    entity->object.rect.x=x;
    entity->object.rect.y=y;
    entity->object.rect.w=32;
    entity->object.rect.h=32;
    entity->hpData.maxHp = 20;
    entity->hpData.currentHp = 20;
    entity->hpData.sizeOfHealthbar = 32;

    /*entity->hpData.healthBarMax.x=x;
    entity->hpData.healthBarMax.y=y+32;
    entity->hpData.healthBarMax.w=32;
    entity->hpData.healthBarMax.h=8;
    entity->hpData.healthBarMax.x=x;
    entity->hpData.healthBarMax.y=y+32;
    entity->hpData.healthBarMax.w=32;
    entity->hpData.healthBarMax.h=8;*/

    /*entity->hpData.healthBarCurrent.x=x;
    entity->hpData.healthBarCurrent.y=y+32;
    entity->hpData.healthBarCurrent.w=32;
    entity->hpData.healthBarCurrent.h=8;
    entity->hpData.healthBarCurrent.x=x;
    entity->hpData.healthBarCurrent.y=y+32;
    entity->hpData.healthBarCurrent.w=32;
    entity->hpData.healthBarCurrent.h=8;*/
    return  *entity;
}

void coPlayerControll(Entity *coPlayer) /// recv data from other client and send to entity coplayer
{
    int ms = SDL_GetTicks();
    int sprite = ms /150 % 3+2;
    //printf("sprite %d",sprite);
    printf("coPlayer->mPosX: %d\n",coPlayer->mPosX);
    printf("coPlayer->mPosX: %d\n",coPlayer->mPosY);
    if(getAIPositionX(coPlayer)>coPlayer->mPosX)
       {
           coPlayer->spriteFacing.x = sprite*32;
           coPlayer->spriteFacing.y = 96;
           coPlayer->spriteFacing.w = 32;
           coPlayer->spriteFacing.h = 32;
        }
    else if(getAIPositionX(coPlayer)<coPlayer->mPosX)
       {
           coPlayer->spriteFacing.x = sprite*32;
           coPlayer->spriteFacing.y = 64;
           coPlayer->spriteFacing.w = 32;
           coPlayer->spriteFacing.h = 32;
       }
    else if(getAIPositionY(coPlayer)>coPlayer->mPosY)
       {
           coPlayer->spriteFacing.x = sprite*32;
           coPlayer->spriteFacing.y = 32;
           coPlayer->spriteFacing.w = 32;
           coPlayer->spriteFacing.h = 32;

       }
    else if(getAIPositionY(coPlayer)<coPlayer->mPosY)
       {
           coPlayer->spriteFacing.x = sprite*32;
           coPlayer->spriteFacing.y = 0;
           coPlayer->spriteFacing.w = 32;
           coPlayer->spriteFacing.h = 32;
       }
}
