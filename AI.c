#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "AI.h"


#define PUBLIC /* empty */
#define PRIVATE static
PRIVATE const int SCREEN_WIDTH = 1366;
PRIVATE const int SCREEN_HEIGHT = 760;
PRIVATE int AI_WIDTH = 15;
PRIVATE int AI_HEIGHT = 15;

struct AI_type{
    int AI_POSITION_X;
    int AI_POSITION_Y;
    int AI_XVELOCITY;
    int AI_YVELOCITY;
    //SDL_Rect Rect;
    int HP;
};
PUBLIC AI createAI(int x, int y){
    AI s = malloc(sizeof(struct AI_type));
    s->AI_POSITION_Y = y;
    s->AI_POSITION_X = x;
    return s;
}
PUBLIC int getAIPositionX(AI a){
    return a->AI_POSITION_X;
}
PUBLIC int getAIPositionY(AI a){
    return a->AI_POSITION_Y;
}
/*PUBLIC void setRectanglePositions(AI a, int x, int y, int width,int height){
    a->Rect.x = x;
    a->Rect.y = y;
    a->Rect.w = width;
    a->Rect.h = height;
}*/
/*PUBLIC void setVelocity(AI a, int xv, int yv){

    a->AI_XVELOCITY = xv;
    a->AI_YVELOCITY = yv;
    return;
}*/
PUBLIC void AITick(AI a){

    if(getAIPositionY(a)>=700)
    {
            (a->AI_POSITION_Y)=(a->AI_POSITION_Y)-rand()%3;
    }
    else
    {
        (a->AI_POSITION_Y)=(a->AI_POSITION_Y)+rand()%3;
    }
    if(getAIPositionX(a)>1300)
    {
        (a->AI_POSITION_X)=(a->AI_POSITION_X)-rand()%3;
    }
    else
    {
       (a->AI_POSITION_X) = (a->AI_POSITION_X)+rand()%3;
    }


    //(a->AI_POSITION_Y)=(a->AI_POSITION_Y)+rand()%3;
    //(a->AI_POSITION_Y)=(a->AI_POSITION_Y)-rand()%2;
    //(a->AI_POSITION_X)=(a->AI_POSITION_X)-rand()%2;
    //(a->AI_POSITION_X)=(a->AI_POSITION_X)+rand()%2;
    /*if(a->AI_POSITION_Y>SCREEN_HEIGHT-100)
    {
        (a->AI_POSITION_X)=(a->AI_POSITION_X)++;
         a->AI_POSITION_Y = SCREEN_HEIGHT-150;
    }
    else if(a->AI_POSITION_Y<100)
    {
        (a->AI_POSITION_X)=(a->AI_POSITION_X)--;
        a->AI_POSITION_Y = 150;
    }
    else if(a->AI_POSITION_X>SCREEN_WIDTH-100)
    {
        (a->AI_POSITION_Y)=(a->AI_POSITION_Y)++;
         a->AI_POSITION_X = SCREEN_WIDTH-150;
    }
    else if(a->AI_POSITION_X<100)
    {
        (a->AI_POSITION_Y)=(a->AI_POSITION_Y)--;
        a->AI_POSITION_X = 150;
    }
   /* else
    {
        (a->AI_POSITION_Y)=(a->AI_POSITION_Y)+ rand() %11 +(-5);
        (a->AI_POSITION_X)=(a->AI_POSITION_X)+ rand() %11 +(-5);
    }
    /*if(getAIPositionX(a)<10)
    {
        (a->AI_POSITION_X)=(a->AI_POSITION_X)++;
    }
    else
    {
        (a->AI_POSITION_Y)=(a->AI_POSITION_Y)++;
    }*/




    /*if(getAIPositionY(a)>=0 || getAIPositionY(a)<=760 )
    {
        (a->AI_POSITION_Y)=(a->AI_POSITION_Y)++;
    }
    else if(getAIPositionY(a)>760)
    {
         (a->AI_POSITION_Y)=(a->AI_POSITION_Y)--;
    }
    else if( getAIPositionY(a)<0)
    {
        (a->AI_POSITION_Y)=(a->AI_POSITION_Y)++;
    }
    else if(getAIPositionX(a)<=1366 || getAIPositionX(a)>=0)
    {
        (a->AI_POSITION_X) = (a->AI_POSITION_X)++;
    }
    else if(getAIPositionX(a)>1366 || getAIPositionX(a)<0)
    {
        (a->AI_POSITION_X) = (a->AI_POSITION_X)--;
    }
    else if(getAIPositionX(a)<0)
    {
        (a->AI_POSITION_X) = (a->AI_POSITION_X)++;
    }*/
    /*if(a->slow == 20){
        //(a->AI_POSITION_Y)=(a->AI_POSITION_Y)+rand()%3;
        (a->AI_POSITION_X)=(a->AI_POSITION_X)++;//rand()%2;
        a->AI_FRAME = 0;
    }else if(a->slow == 30){
        (a->AI_POSITION_Y)=(a->AI_POSITION_Y)++;//rand()++;//%3;
        (a->AI_POSITION_X)=(a->AI_POSITION_X)++;//rand()++;//%2;
        a->AI_FRAME = 1;
        a->slow = 0;
    }
    (a->slow)++;*/

}



