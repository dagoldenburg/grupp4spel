#ifndef AI_h
#define AI_h

#include <stdio.h>
#include <SDL.h>

typedef struct AI_type *AI;

AI createAI(int x, int y);
int getAIPositionX(AI a);
int getAIPositionY(AI a);
void setRectanglePositions(AI a, int x, int y, int width,int height);
//void setVelocity(AI a, int xv, int yv);
void AITick(AI a);


#endif /* Alien_h */


