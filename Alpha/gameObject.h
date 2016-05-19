#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include "gameStruct.h"
Entity createEntity(Entity *entity,int x ,int y, int w, int h, float hp);
void coPlayerControll(Entity *coPlayer);
#endif // GAMEOBJECT_H_INCLUDED
