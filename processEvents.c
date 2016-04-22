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
    return game->Dot.mPosX;
}
int getmPosY(GameState *game)
{
    return game->Dot.mPosY;
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

    const Uint8 *state = SDL_GetKeyboardState(NULL); //Flyttar på spelaren om det inte finns hinder
    if(state[SDL_SCANCODE_LEFT])
    {
        if(playfield[(game->Dot.mPosY + 10)/TILESIZE][(game->Dot.mPosX -10)/TILESIZE]!=3 //Kontrollerar om det finns ett hinder, annars flyttar spelaren
           &&  playfield[(game->Dot.mPosY)/TILESIZE][(game->Dot.mPosX  -10)/TILESIZE]!=3) {
            game->Dot.mPosX -= 5;
        }
    }
    if(state[SDL_SCANCODE_RIGHT])
    {
        if(playfield[(game->Dot.mPosY + 10)/TILESIZE][(game->Dot.mPosX + 20)/TILESIZE]!=3//Kontrollerar om det finns ett hinder, annars flyttar spelaren
           &&  playfield[(game->Dot.mPosY)/TILESIZE][(game->Dot.mPosX + 20)/TILESIZE]!=3) {
            game->Dot.mPosX += 5;
        }
    }
    if(state[SDL_SCANCODE_UP])
    {
        if(playfield[(game->Dot.mPosY - 10)/TILESIZE][(game->Dot.mPosX + 10)/TILESIZE]!=3//Kontrollerar om det finns ett hinder, annars flyttar spelaren
           &&  playfield[(game->Dot.mPosY - 10)/TILESIZE][(game->Dot.mPosX + 10)/TILESIZE]!=3) {
              game->Dot.mPosY -= 5;
        }
    }
    if(state[SDL_SCANCODE_DOWN])
    {
        if(playfield[(game->Dot.mPosY + 20)/TILESIZE][(game->Dot.mPosX + 10)/TILESIZE]!=3//Kontrollerar om det finns ett hinder, annars flyttar spelaren
           &&  playfield[(game->Dot.mPosY + 20)/TILESIZE][(game->Dot.mPosX + 10)/TILESIZE]!=3) {
              game->Dot.mPosY += 5;
        }
    }

    if( game->Dot.mPosX<0 )
    {
        game->Dot.mPosX=0;
    }
    if( game->Dot.mPosX+20>LEVEL_WIDTH )
    {
        game->Dot.mPosX=LEVEL_WIDTH-20;
    }

    if( game->Dot.mPosY<0  )
    {
        game->Dot.mPosY=0;
    }
      if( game->Dot.mPosY+20>LEVEL_HEIGHT )
    {
        game->Dot.mPosY=LEVEL_HEIGHT-20;
    }



    printf("camera: %d\n",game->Dot.mPosX );
    printf("man: %d\n",game->Dot.mPosY );

  return done;

}
