#include <SDL.h>
//#include <SDL_image.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "playField.h"
#include "gameStruct.h"
#include "loadMedia.h"
#include "processEvents.h"
#include "player.h"
#include "collision.h"


void PlayerWallCollision(GameState *game){

    if( game->Entity.rect.x<0 ){ // Collision med v�nstra delen av rutan
        game->Entity.rect.x=0;
    }
    if( game->Entity.rect.x+TILESIZE + PLAYER_SPEED>LEVEL_WIDTH ){ // Collision med h�gra delen av rutan
        game->Entity.rect.x=LEVEL_WIDTH-TILESIZE - PLAYER_SPEED;
    }

    if( game->Entity.rect.y<0  ){ // Collision med �vre delen av  rutan
        game->Entity.rect.y=0;
    }
    if( game->Entity.rect.y+TILESIZE + PLAYER_SPEED>LEVEL_HEIGHT ){ // Collision med nedre delen av rutan
        game->Entity.rect.y=LEVEL_HEIGHT-TILESIZE - PLAYER_SPEED;
    }

}

/*
Collision detection med en tile.
Den tar reda p� en objects position och delar det med tilesize och skickar
in det i playfield arrayen p� playField.c och f�r reda p� vilken tile objectet
befinner sig i. Om tilen �r lika med v�rdet i arrayen collisionTiles s� returnar
det 1. Player_Speed �r d�r f�r att kompensera f�r �ndrade positionen innan funktionen kallas.
*/

int collisionDetection(Entity *object){
    int LEFT = object->rect.x;//L�ttare att f�rst�
    int TOP = object->rect.y;
    int RIGHT = object->rect.x + object->rect.w;
    int BOTTOM = object->rect.y + object->rect.h;

    for(int i = 0; i<TOTAL_COLLISION_TILES; i++) {
            if(playfield[(TOP + PLAYER_SPEED)/TILESIZE][(LEFT + PLAYER_SPEED)/TILESIZE]==collisionTiles[i]) //COLLISION MED �VRE V�NSTER KANT
            {
                return 1;
            }
            if(playfield[(BOTTOM - PLAYER_SPEED)/TILESIZE][(RIGHT - PLAYER_SPEED)/TILESIZE]==collisionTiles[i])//COLLISION MED NEDRE H�GER KANT
            {
                return 1;
            }

            if(playfield[(TOP + PLAYER_SPEED)/TILESIZE][(RIGHT - PLAYER_SPEED)/TILESIZE]==collisionTiles[i])//COLLISION MED �VRE H�GER KANT
            {
                return 1;
            }

            if(playfield[(BOTTOM - PLAYER_SPEED)/TILESIZE][(LEFT + PLAYER_SPEED)/TILESIZE]==collisionTiles[i]) //COLLISION MED NEDRE V�NSTER KANT
            {
                return 1;
            }
    }
    return 0;
}

int attackCollision(Entity *player, Entity *AI){
    if(player->attack.x >= AI->rect.x && player->attack.x <= AI->rect.x + AI->rect.w &&
       player->attack.y >= AI->rect.y && player->attack.y <= AI->rect.y + AI->rect.h){
        return 1;
    }

    if(player->attack.x + player->attack.w>= AI->rect.x && player->attack.x + player->attack.w <= AI->rect.x + AI->rect.w &&
       player->attack.y >= AI->rect.y && player->attack.y <= AI->rect.y + AI->rect.h){
        return 1;
    }

    if(player->attack.x >= AI->rect.x && player->attack.x <= AI->rect.x + AI->rect.w &&
       player->attack.y + player->attack.h>= AI->rect.y && player->attack.y + player->attack.h <= AI->rect.y + AI->rect.h){
        return 1;
    }

    if(player->attack.x + player->attack.w>= AI->rect.x && player->attack.x + player->attack.w <= AI->rect.x + AI->rect.w &&
       player->attack.y + player->attack.h>= AI->rect.y && player->attack.y + player->attack.h <= AI->rect.y + AI->rect.h){
        return 1;
    }

    return 0;

}
