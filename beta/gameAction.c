#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include "gameStruct.h"
#include "main.h"
#include "playField.h"
#include "gameAction.h"
#include "gameObject.h"
int getmPosX(Entity *entity)
{
    return entity->object.rect.x;
}
int getmPosY(Entity *entity)
{
    return entity->object.rect.y;
}


void aiDead(GameState *gamestate, Entity *entity){
    char string[31];

    sprintf(string,"aidead!, ID:00%d", entity->id);

    stringManipulate(entity->id,14,string);

    string[29] = '\n'; //newline
    string[30] = '\0'; // null
    gamestate->aiEntityToken[entity->id] = 0;
    printf("ai Death %d token %d\n",entity[entity->id],gamestate->aiEntityToken[entity->id]);
    safeSend(string,gamestate);
    return;
}

void updatePlayerMovement(GameState *gamestate){
        char string[30];
        int x,y;
        char xArr[4],yArr[4];
        sprintf(string,"movplyr, ID:00%d x:0000 y:0000",gamestate->mySlot);

        x = getmPosX(&gamestate->playerEntity[gamestate->mySlot]);
        stringManipulate(x,21,string);

        y = getmPosY(&gamestate->playerEntity[gamestate->mySlot]);
        stringManipulate(y,28,string);

        safeSend(string,gamestate);
    return;
}

void giveDamage(GameState *gamestate,int i){
    char string[30];
    int hp;
    strcpy(string,"givedmg, ID:000 hp:0000......");
    gamestate->AiEntity[i].hpData.currentHp -= gamestate->playerEntity[gamestate->mySlot].strength;
    hp = gamestate->AiEntity[i].hpData.currentHp;
    stringManipulate(i,14,string);
    stringManipulate(hp,22,string);
    safeSend(string,gamestate);
    return;

}



void spawnAttack(Entity *playerEntity) {
    playerEntity->attack = playerEntity->object.rect;

    if(playerEntity->spriteFacing.y == 0){ //Up
        playerEntity->attack.x -= TILESIZE/4;
        playerEntity->attack.w += TILESIZE/2;
        playerEntity->attack.y -= TILESIZE;
    }

    if(playerEntity->spriteFacing.y == 32){ //Down
        playerEntity->attack.x -= TILESIZE/4;
        playerEntity->attack.w += TILESIZE/2;
        playerEntity->attack.y += TILESIZE;
    }

    if(playerEntity->spriteFacing.y == 96){ //Right
        playerEntity->attack.x += TILESIZE;
        playerEntity->attack.h += TILESIZE/2;
        playerEntity->attack.y -= TILESIZE/4;
    }

    if(playerEntity->spriteFacing.y == 64){ //Left
        playerEntity->attack.x -= TILESIZE;
        playerEntity->attack.h += TILESIZE/2;
        playerEntity->attack.y -= TILESIZE/4;
    }

return;
}

void controlplayer(Entity *playerEntity)
{
    int ms = SDL_GetTicks();
    int sprite = ms /3 % 3;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_LEFT])
    {
        playerEntity->spriteFacing.x=sprite*32;
        playerEntity->spriteFacing.y=64;

        playerEntity->object.rect.x -= PLAYER_SPEED;
        if(TilemapCollisionDetection(playerEntity->object.rect))
        {
           playerEntity->object.rect.x += PLAYER_SPEED;
        }
    }
    if(state[SDL_SCANCODE_RIGHT])
    {

        playerEntity->spriteFacing.x=sprite*32;
        playerEntity->spriteFacing.y=96;
        playerEntity->object.rect.x += PLAYER_SPEED;
        if(TilemapCollisionDetection(playerEntity->object.rect))
        {
           playerEntity->object.rect.x -= PLAYER_SPEED;
        }
    }
    if(state[SDL_SCANCODE_UP])
    {
        playerEntity->spriteFacing.x=sprite*32;
        playerEntity->spriteFacing.y=0;
        playerEntity->object.rect.y -= PLAYER_SPEED;
        if(TilemapCollisionDetection(playerEntity->object.rect))
        {
          playerEntity->object.rect.y += PLAYER_SPEED;
        }
    }
    if(state[SDL_SCANCODE_DOWN])
    {
        playerEntity->spriteFacing.x=sprite*32;
        playerEntity->spriteFacing.y=32;
        playerEntity->object.rect.y += PLAYER_SPEED;
        if(TilemapCollisionDetection(playerEntity->object.rect))
        {
           playerEntity->object.rect.y -= PLAYER_SPEED;
        }
    }
    if(state[SDL_SCANCODE_SPACE]){
        if(playerEntity->attackTimer == 0 || (SDL_GetTicks() - playerEntity->attackTimer) >100){
            spawnAttack(playerEntity);
            playerEntity->attackTimer = SDL_GetTicks();
        }
    }

    playWallCollision(&playerEntity->object);
        ///*********************player colliction detector*************************////

}

int checkIFObjectHit(SDL_Rect *playerRect, SDL_Rect *AiRect){
    if (playerRect->x <= AiRect->x + AiRect->w&&
       playerRect->x + playerRect->w >= AiRect->x &&
       playerRect->y <= AiRect->y + AiRect->h &&
       playerRect->y + playerRect->w >= AiRect->y) {
        return 1;
    }
    return 0;
}
int checkIFAiDead(Entity *player, Entity *AI, GameState *gamestate){
    if((AI->hpData.currentHp -= player->strength) <0) {
         giveDamage(gamestate, AI);
        return 1;
    }
    giveDamage(gamestate, AI);
    return 0;

}


void resetAttack(SDL_Rect *attack){
    attack->h = 0;
    attack->w = 0;
    attack->x = 0;
    attack->y = 0;
}

void playWallCollision(gameObject *object)
{


    if( object->rect.x<0 )
    {
       object->rect.x=0;
    }
    if( object->rect.x+20>LEVEL_WIDTH )
    {

        object->rect.x=LEVEL_WIDTH-20;
    }

    if( object->rect.y<0  )
    {
       object->rect.y=0;
    }
    if( object->rect.y+20>LEVEL_HEIGHT )
    {
        object->rect.y=LEVEL_HEIGHT-20;
    }

}
int TilemapCollisionDetection(SDL_Rect rect){
    int LEFT = rect.x;//Lättare att förstå
    int TOP = rect.y;
    int RIGHT = rect.x + rect.w;
    int BOTTOM = rect.y + rect.h;

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
int processEvents(SDL_Window *window, GameState *game)
{
    SDL_Event event;
    int done = 0;

    while(SDL_PollEvent(&event))
    {
    switch(event.type)
    {
      case SDL_WINDOWEVENT_CLOSE:
      {
        if(window)
        {
          SDL_DestroyWindow(window);
          window = NULL;
          done = 1;
        }
      }
      break;
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            //done = 1;
          break;
        }
      }
      break;
      case SDL_QUIT:
        //quit out of the game
        done = 1;
      break;
    }
    }

    controlplayer(&game->playerEntity[game->mySlot]);
    //printf("camera: %d\n",game->playerEntity[0].object.rect.x );
    //printf("man: %d\n",game->playerEntity[0].object.rect.y );

  return done;

}

