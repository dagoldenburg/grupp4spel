#include "SDL.h"
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "playField.h"
#include "gameStruct.h"
#include "loadMedia.h"
#include "processEvents.h"

int getmPosX(GameState *game)
{
    return game->Entity.mPosX;
}
int getmPosY(GameState *game)
{
    return game->Entity.mPosY;
}

int collisionDetection(Entity *object){
    for(int i = 0; i<TOTAL_COLLISION_TILES; i++) {
            if(playfield[(object->mPosY + 5)/TILESIZE][(object->mPosX + 5)/TILESIZE]==collisionTiles[i]) //Kontrollerar om det finns ett hinder, annars flyttar objektet
            {
                printf("Upper box\n");
                return 1;
            }
            if(playfield[(object->mPosY + TILESIZE - 5)/TILESIZE][(object->mPosX + TILESIZE - 5)/TILESIZE]==collisionTiles[i])
            {
                printf("Lower box\n");
                return 1;
            }

            if(playfield[(object->mPosY + 5)/TILESIZE][(object->mPosX + TILESIZE - 5)/TILESIZE]==collisionTiles[i])
            {
                printf("Right box\n");
                return 1;
            }

            if(playfield[(object->mPosY + TILESIZE - 5)/TILESIZE][(object->mPosX + 5)/TILESIZE]==collisionTiles[i])
            {
                printf("Left box\n");
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

    const Uint8 *state = SDL_GetKeyboardState(NULL); //Flyttar p� spelaren om det inte finns hinder(Collision detection)
    if(state[SDL_SCANCODE_LEFT])
    {
        game->Entity.mPosX -= PLAYER_SPEED;

        if(collisionDetection(&game->Entity)) {
           game->Entity.mPosX += PLAYER_SPEED;
        }


    }

    if(state[SDL_SCANCODE_RIGHT])
    {
        game->Entity.mPosX += PLAYER_SPEED;

        if(collisionDetection(&game->Entity)){
           game->Entity.mPosX -= PLAYER_SPEED;
        }
    }

    if(state[SDL_SCANCODE_UP])
    {
        game->Entity.mPosY -= PLAYER_SPEED;

        if(collisionDetection(&game->Entity)){
           game->Entity.mPosY += PLAYER_SPEED;
        }
    }

    if(state[SDL_SCANCODE_DOWN])
    {
        game->Entity.mPosY += PLAYER_SPEED;

        if(collisionDetection(&game->Entity)){
           game->Entity.mPosY -= PLAYER_SPEED;
        }
    }


    if( game->Entity.mPosX<0 )
    {
        game->Entity.mPosX=0;
    }
    if( game->Entity.mPosX+TILESIZE + PLAYER_SPEED>LEVEL_WIDTH )
    {
        game->Entity.mPosX=LEVEL_WIDTH-TILESIZE - PLAYER_SPEED;
    }

    if( game->Entity.mPosY<0  )
    {
        game->Entity.mPosY=0;
    }
      if( game->Entity.mPosY+TILESIZE + PLAYER_SPEED>LEVEL_HEIGHT )
    {
        game->Entity.mPosY=LEVEL_HEIGHT-TILESIZE - PLAYER_SPEED;
    }



    //printf("manx: %d\n",game->Entity.mPosX );
    //printf("many: %d\n",game->Entity.mPosY );

  return done;

}
