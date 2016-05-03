#include <SDL.h>
#include "main.h"
#include "gameStruct.h"
#include "createEntity.h"

GameState createEntity(GameState entity, int x, int y, SDL_Renderer *renderer) {
    entity.Entity.rect.x=x;
    entity.Entity.rect.y=y;
    entity.Entity.rect.h=32;
    entity.Entity.rect.w=32;
    entity.Entity.mVelX=0;
    entity.Entity.mVelY=0;
    entity.renderer=renderer;
    entity.Entity.hpData.maxHp = 100.0;
    entity.Entity.hpData.currentHp = 50.0;
    entity.Entity.hpData.sizeOfHealthbar = 32;
    entity.Entity.hpData.healthBarCurrent.h = 8;
    entity.Entity.hpData.healthBarMax.h = 8;

    return entity;
}
int getAIPositionX(GameState *entity){
    return entity->Entity.rect.x;
}
int getAIPositionY(GameState *entity){
    return entity->Entity.rect.y;
}
