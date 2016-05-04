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

    if( game->Entity.rect.x<0 ){ // Collision med vänstra delen av rutan
        game->Entity.rect.x=0;
    }
    if( game->Entity.rect.x+TILESIZE + PLAYER_SPEED>LEVEL_WIDTH ){ // Collision med högra delen av rutan
        game->Entity.rect.x=LEVEL_WIDTH-TILESIZE - PLAYER_SPEED;
    }

    if( game->Entity.rect.y<0  ){ // Collision med övre delen av  rutan
        game->Entity.rect.y=0;
    }
    if( game->Entity.rect.y+TILESIZE + PLAYER_SPEED>LEVEL_HEIGHT ){ // Collision med nedre delen av rutan
        game->Entity.rect.y=LEVEL_HEIGHT-TILESIZE - PLAYER_SPEED;
    }

}

/*
Collision detection med en tile.
Den tar reda på en objects position och delar det med tilesize och skickar
in det i playfield arrayen på playField.c och får reda på vilken tile objectet
befinner sig i. Om tilen är lika med värdet i arrayen collisionTiles så returnar
det 1. Player_Speed är där för att kompensera för ändrade positionen innan funktionen kallas.
*/

int collisionDetection(Entity *object){
    int LEFT = object->rect.x;//Lättare att förstå
    int TOP = object->rect.y;
    int RIGHT = object->rect.x + object->rect.w;
    int BOTTOM = object->rect.y + object->rect.h;

    for(int i = 0; i<TOTAL_COLLISION_TILES; i++) {
            if(playfield[(TOP + PLAYER_SPEED)/TILESIZE][(LEFT + PLAYER_SPEED)/TILESIZE]==collisionTiles[i]) //COLLISION MED ÖVRE VÄNSTER KANT
            {
                return 1;
            }
            if(playfield[(BOTTOM - PLAYER_SPEED)/TILESIZE][(RIGHT - PLAYER_SPEED)/TILESIZE]==collisionTiles[i])//COLLISION MED NEDRE HÖGER KANT
            {
                return 1;
            }

            if(playfield[(TOP + PLAYER_SPEED)/TILESIZE][(RIGHT - PLAYER_SPEED)/TILESIZE]==collisionTiles[i])//COLLISION MED ÖVRE HÖGER KANT
            {
                return 1;
            }

            if(playfield[(BOTTOM - PLAYER_SPEED)/TILESIZE][(LEFT + PLAYER_SPEED)/TILESIZE]==collisionTiles[i]) //COLLISION MED NEDRE VÄNSTER KANT
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
