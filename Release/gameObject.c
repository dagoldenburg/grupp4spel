#include "main.h"
#include "playField.h"
#include "gameObject.h"
#include "gameAction.h"
#include "time.h"

/*
Skapar Entitet i minnet och tillsätter alla datamedlemmarna i strukten.
*/

Entity createEntity(Entity *entity,int x ,int y,int w,int h, float hp)
{
    entity->mVelX=0;
    entity->mVelY=0;
    entity->spriteFacing.x=0;
    entity->spriteFacing.y=0;
    entity->spriteFacing.w=TILESIZE;
    entity->spriteFacing.h=TILESIZE;
    entity->object.rect.x=x; //Entitetens rektangel som beskriver vart den ligger i spelplanen
    entity->object.rect.y=y;
    entity->object.rect.w=w;
    entity->object.rect.h=h;
    entity->hpData.maxHp = hp;
    entity->hpData.currentHp = hp;
    entity->hpData.sizeOfHealthbar = w;
    entity->strength = 5;
    entity->deathAnimation = 0;

    entity->attack.rect.h = 0;
    entity->attack.rect.w = 0;
    entity->attack.rect.x = 0;
    entity->attack.rect.y = 0;

    entity->attack.spriteFacing.h = 0;
    entity->attack.spriteFacing.w = 0;
    entity->attack.spriteFacing.x = 0;
    entity->attack.spriteFacing.y = 0;
    entity->attack.velX = 0;
    entity->attack.velY = 0;

    entity->hpData.healthBarMax.x=x;
    entity->hpData.healthBarMax.y=y+TILESIZE;
    entity->hpData.healthBarMax.w=w;
    entity->hpData.healthBarMax.h = TILESIZE / 4;

    entity->hpData.healthBarCurrent.x=x;
    entity->hpData.healthBarCurrent.y=y+TILESIZE;
    entity->hpData.healthBarCurrent.w=w;
    entity->hpData.healthBarCurrent.h= TILESIZE /4;

    entity->characterType = 0; //Default för warrior
    strcpy(entity->name, "Mitt namn");
    return  *entity;
}

