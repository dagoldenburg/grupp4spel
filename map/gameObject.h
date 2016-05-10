#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include "gameStruct.h"
Entity createEntity(Entity *entity,int x ,int y);
void coPlayerControll(Entity *coPlayer);
void sortAIArray(Entity AIArray[], int n, int noOfAI);
#endif // GAMEOBJECT_H_INCLUDED
