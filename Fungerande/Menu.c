#include <SDL.h>
#include "main.h"

void startMenu(SDL_Renderer *renderer)
{
   int mX,mY,quit=0;
   SDL_Surface *loadedSurface = IMG_Load("PixelMenu.png");
   SDL_Event event;
    if(loadedSurface==NULL)
    {
        printf("%s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    SDL_Texture *menuTexture = SDL_CreateTextureFromSurface(renderer,loadedSurface);
    if(menuTexture == NULL)
    {
        printf("%s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
   if(SDL_RenderCopy(renderer,menuTexture,NULL,NULL) !=0)
   {
       printf("%s RenderCopy fail\n", SDL_GetError());
        SDL_Quit();
        exit(1);

   }
   while(!quit)
   {
        while(SDL_PollEvent(&event))
        {
            SDL_PumpEvents();
            SDL_GetMouseState(&mX,&mY);
            if( event.type == SDL_QUIT )
            {
              SDL_Quit;
              exit(1);
            }
           else if(event.type == SDL_MOUSEBUTTONDOWN && (mX>560 && mX<715))
            {
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    if(mY>300 && mY<325)
                    {
                        quit=1;
                    }
                    else if(mY>420 && mY<470)
                    {
                        SDL_Quit();
                        exit(1);
                    }
                }

            }
        }
        SDL_RenderPresent(renderer);
   }

        SDL_FreeSurface(loadedSurface);
        SDL_DestroyTexture(menuTexture);
return;
}
