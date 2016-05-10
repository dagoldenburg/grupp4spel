#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>

#elif __linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#endif // _WIN32
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

void controlplayer(Entity *playerEntity)
{
    int ms = SDL_GetTicks();
    int sprite = ms /3 % 3;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_LEFT])
    {
        playerEntity->spriteFacing.x=sprite*32;
        playerEntity->spriteFacing.y=64;

        playerEntity->object.rect.x -= 2;
        if(TilemapCollisionDetection(playerEntity->object.rect))
        {
           playerEntity->object.rect.x += PLAYER_SPEED;
        }
    }
    if(state[SDL_SCANCODE_RIGHT])
    {

        playerEntity->spriteFacing.x=sprite*32;
        playerEntity->spriteFacing.y=96;
        playerEntity->object.rect.x += 2;
        if(TilemapCollisionDetection(playerEntity->object.rect))
        {
           playerEntity->object.rect.x -= PLAYER_SPEED;
        }
    }
    if(state[SDL_SCANCODE_UP])
    {
        playerEntity->spriteFacing.x=sprite*32;
        playerEntity->spriteFacing.y=0;
        playerEntity->object.rect.y -= 2;
        if(TilemapCollisionDetection(playerEntity->object.rect))
        {
          playerEntity->object.rect.y += PLAYER_SPEED;
        }
    }
    if(state[SDL_SCANCODE_DOWN])
    {
        playerEntity->spriteFacing.x=sprite*32;
        playerEntity->spriteFacing.y=32;
        playerEntity->object.rect.y += 2;
        if(TilemapCollisionDetection(playerEntity->object.rect))
        {
           playerEntity->object.rect.y -= PLAYER_SPEED;
        }

    }

    if(state[SDL_SCANCODE_SPACE]){ // Spelaren går Attackerar
        if(playerEntity->attackTimer == 0 || (SDL_GetTicks() - playerEntity->attackTimer) >100){
            spawnAttack(playerEntity);
            playerEntity->attackTimer = SDL_GetTicks();
        }
    }
    playWallCollision(&playerEntity->object );
        ///*********************player colliction detector*************************////

}

void spawnAttack(Entity *playerEntity) {
    playerEntity->attack = playerEntity->object.rect;

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


int checkIFAttackHit(SDL_Rect playerRect, SDL_Rect AiRect){
    if(playerRect.x >= AiRect.x && playerRect.x <= AiRect.x + AiRect.w &&
       playerRect.y >= AiRect.y && playerRect.y <= AiRect.y + AiRect.h){

        return 1;
    }
    if(playerRect.x + playerRect.w>= AiRect.x && playerRect.x + playerRect.w <= AiRect.x + AiRect.w &&
       playerRect.y >= AiRect.y && playerRect.y <= AiRect.y + AiRect.h){
           printf("YES");
        return 1;
    }

    if(playerRect.x >= AiRect.x && playerRect.x <= AiRect.x + AiRect.w &&
       playerRect.y + playerRect.h>= AiRect.y && playerRect.y + playerRect.h <= AiRect.y + AiRect.h){
           printf("YES");
        return 1;
    }

    if(playerRect.x + playerRect.w>= AiRect.x && playerRect.x + playerRect.w <= AiRect.x + AiRect.w &&
       playerRect.y + playerRect.h>= AiRect.y && playerRect.y + playerRect.h <= AiRect.y + AiRect.h){
           printf("YES");
        return 1;
    }

    return 0;

}


int checkIFAiDead(Entity *player, Entity *AI){
    if(player->spriteFacing.x == 32 && player->spriteFacing.y == 32){
        AI->object.rect.x -= 5;
        if(TilemapCollisionDetection(AI->object.rect)){
            AI->object.rect.x += 5;
        }
    }
    if(player->spriteFacing.x == 0 && player->spriteFacing.y == 32){
        AI->object.rect.x += 10;
        if(TilemapCollisionDetection(AI->object.rect)){
            AI->object.rect.x -= 5;
        }
    }
    if(player->spriteFacing.x == 0 && player->spriteFacing.y == 0){
        AI->object.rect.y -= 5;
        if(TilemapCollisionDetection(AI->object.rect)){
            AI->object.rect.y += 5;
        }

    }
    if(player->spriteFacing.x == 32 && player->spriteFacing.y == 0){
        AI->object.rect.y += 5;
        if(TilemapCollisionDetection(AI->object.rect)){
            AI->object.rect.y -= 5;
        }
    }

    resetAttack(&player->attack);
    if((AI->hpData.currentHp -= player->strength) <0) {
        return 1;
    }

    return 0;

}


void resetAttack(SDL_Rect *attack){
    attack->h = 0;
    attack->w = 0;
    attack->x = 0;
    attack->y = 0;
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
            done = 1;
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

    controlplayer(&game->playerEntity[0]);
    //coPlayerControll(&game->playerEntity[1]);
    //printf("camera: %d\n",game->playerEntity[0].object.rect.x );
    //printf("man: %d\n",game->playerEntity[0].object.rect.y );

  return done;

}
