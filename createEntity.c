#include <SDL.h>
#include "main.h"
#include "ai.h"
#include "gameStruct.h"

GameState* creatyEntity(Gamestate *entity, int x, int y, SDL_Renderer *renderer) {
    entity.rect.x=x;
    entity.rect.y=y;
    entity.rect.h=32;
    entity.rect.w=32;
    entity.mVelX=0;
    entity.mVelY=0;
    Entity.renderer=renderer;
    entity.hpData.maxHp = 100.0;
    entity.hpData.currentHp = 50.0;
    entity.hpData.sizeOfHealthbar = 32;
    entity.hpData.healthBarCurrent.h = 8;
    entity.hpData.healthBarMax.h = 8;

    return &entity;
}
int getAIPositionX(GameState *entity){
    return entity.Entity.rect.x;
}
int getAIPositionY(GameState *entity){
    return entity.Entity.rect.y;
}
