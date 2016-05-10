#include "gameAI.h"
#include "gameStruct.h"
#include "gameAction.h"
#include <time.h>
int getAIPositionX(Entity *entity){
    //printf("AIposX %d\n",entity->object.rect.x);
    return entity->object.rect.x;
}
int getAIPositionY(Entity *entity){
    //printf("AIposy %d\n",entity->object.rect.y);
    return entity->object.rect.y;
}

void whatSprite(Entity *AI)
{
    int ms = SDL_GetTicks();
    int sprite = ms /150 % 3+1;
    //printf("sprite %d",sprite);
    printf("AI->mPosX: %d\n",AI->mPosX);
    printf("AI->mPosX: %d\n",AI->mPosY);
    if(getAIPositionX(AI)>AI->mPosX)
       {
           AI->spriteFacing.x = sprite*32;
           AI->spriteFacing.y = 96;
           AI->spriteFacing.w = 32;
           AI->spriteFacing.h = 32;
        }
    else if(getAIPositionX(AI)<AI->mPosX)
       {
           AI->spriteFacing.x = sprite*32;
           AI->spriteFacing.y = 64;
           AI->spriteFacing.w = 32;
           AI->spriteFacing.h = 32;
       }
    else if(getAIPositionY(AI)>AI->mPosY)
       {
           AI->spriteFacing.x = sprite*32;
           AI->spriteFacing.y = 32;
           AI->spriteFacing.w = 32;
           AI->spriteFacing.h = 32;

       }
    else if(getAIPositionY(AI)<AI->mPosY)
       {
           AI->spriteFacing.x = sprite*32;
           AI->spriteFacing.y = 0;
           AI->spriteFacing.w = 32;
           AI->spriteFacing.h = 32;
       }

    playWallCollision(&AI->object);

}
void AITTick (Entity *entity)
{
    entity->object.rect.x=entity->object.rect.x-entity->mVelX;
    if(TilemapCollisionDetection(entity->object.rect))
    {
       entity->mVelX=0;
       entity->mVelY=0;
    }

}
