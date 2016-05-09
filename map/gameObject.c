#include "main.h"
#include "playField.h"
#include "gameObject.h"
#include "gameAction.h"
#include "time.h"
Entity createEntity(Entity *entity,int x ,int y)
{

    entity->mVelX=2;
    entity->mVelY=2;
    entity->spriteFacing.w=32;
    entity->spriteFacing.h=32;
    entity->mPosX=0;
    entity->mPosY=0;
    entity->object.rect.x=x;
    entity->object.rect.y=y;
    entity->object.rect.w=32;
    entity->object.rect.h=32;

    return  *entity;
}

void coPlayerControll(Entity *coPlayer) /// recv data from other client and send to entity coplayer
{
    int ms = SDL_GetTicks();
    int sprite = ms /3 % 3;
     sscanf(recvbuffer,"%d %d",&coPlayer->object.rect.x,&coPlayer->object.rect.y);
     if (getmPosX(coPlayer)>coPlayer->mPosX)  ///  check if coplayer  x-postion increments
     {
         coPlayer->spriteFacing.x=sprite*32;
         coPlayer->spriteFacing.y=96;
         coPlayer->spriteFacing.w=32;
         coPlayer->spriteFacing.h=32;
     }
      if (getmPosX(coPlayer)<coPlayer->mPosX)///  check if coplayer  x-postion decrements
     {
         coPlayer->spriteFacing.x=sprite*32;
         coPlayer->spriteFacing.y=64;
         coPlayer->spriteFacing.w=32;
         coPlayer->spriteFacing.h=32;
     }

     if (getmPosY(coPlayer)>coPlayer->mPosY) ///  check if coplayer  y-postion increments
     {
         coPlayer->spriteFacing.x=sprite*32;
         coPlayer->spriteFacing.y=32;
         coPlayer->spriteFacing.w=32;
         coPlayer->spriteFacing.h=32;
     }
      if (getmPosY(coPlayer)<coPlayer->mPosY) /// check if coplayer  x-postion decrements
     {
         coPlayer->spriteFacing.x=sprite*32;
         coPlayer->spriteFacing.y=0;
         coPlayer->spriteFacing.w=32;
         coPlayer->spriteFacing.h=32;
     }
}
