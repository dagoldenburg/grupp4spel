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

void updatePlayerMovement(GameState *gamestate){
        char string[100];
        int x,y;
        char xArr[4],yArr[4];
        sprintf(string,"movplyr, ID:00%d x:0000 y:0000",gamestate->mySlot);

        x = getmPosX(&gamestate->playerEntity[gamestate->mySlot]);
        snprintf(xArr, 4, "%d",x); // gör inten till en string som man kan lägga in i spawnai meddelandet
        switch(x){ // korrigerar så att man skickar rätt siffra på AIn, så man kan urskilja dem.
        case 0 ... 9: string[21] = xArr[0];
                      break;
        case 10 ... 99: string[21] = xArr[1];
                        string[20] = xArr[0];
                        break;
        case 100 ... 999: string[21] = xArr[2];
                          string[20] = xArr[1];
                          string[19] = xArr[0];
                          break;
        case 1000 ... 9999: string[21] = xArr[3];
                            string[20] = xArr[2];
                            string[19] = xArr[1];
                            string[18] = xArr[0];
                            break;
        }
        y = getmPosY(&gamestate->playerEntity[gamestate->mySlot]);
        snprintf(yArr, 4, "%d",y); // gör inten till en string som man kan lägga in i spawnai meddelandet
        switch(y){ // korrigerar så att man skickar rätt siffra på AIn, så man kan urskilja dem.
        case 0 ... 9: string[28] = yArr[0];
                      break;
        case 10 ... 99: string[28] = yArr[1];
                        string[27] = yArr[0];
                        break;
        case 100 ... 999: string[28] = yArr[2];
                          string[27] = yArr[1];
                          string[26] = yArr[0];
                          break;
        case 1000 ... 9999: string[28] = yArr[3];
                            string[27] = yArr[2];
                            string[26] = yArr[1];
                            string[15] = yArr[0];
                            break;
        }
        string[29] = '\n'; //newline
        string[30] = '\0'; // null
        printf("trying to send player movement\n");
        safeSend(string,gamestate);
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
    playWallCollision(&playerEntity->object);
        ///*********************player colliction detector*************************////

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

    controlplayer(&game->playerEntity[game->mySlot]);
    //printf("camera: %d\n",game->playerEntity[0].object.rect.x );
    //printf("man: %d\n",game->playerEntity[0].object.rect.y );

  return done;

}

