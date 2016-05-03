#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <SDL_image.h>
#include "AI.h"
#define INT_MAX 32767


const int SCREEN_WIDTH = 1366; //1366;
const int SCREEN_HEIGHT = 760;//760;
//typedef struct AI_type ai;
AI aiArray[100];
SDL_Rect aiRect[100];
bool init();

void close();

SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gXOut = NULL;

SDL_Renderer *gRenderer = NULL;

SDL_Texture* gTexture = NULL;

SDL_Texture* loadTexture(char string[]);


bool init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow( "AI", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{



            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED);

            if(gRenderer== NULL)
            {
                printf("ABCD%s",SDL_GetError());
            }
			gScreenSurface = SDL_GetWindowSurface( gWindow );

			SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0xEE, 0xEE, 0xEE ) );

			SDL_UpdateWindowSurface(gWindow);


		}
	}

	return success;
}
void close()
{
	SDL_FreeSurface( gXOut );
	gXOut = NULL;

    SDL_DestroyTexture( gTexture );
	gTexture = NULL;

	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}
bool loadMedia()
{
	bool success = true;

	gTexture = loadTexture("Sprite.png");
	if( gTexture == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "C:/Users/nti/Desktop/AI/Sprite.png", SDL_GetError() );
		success = false;
	}

	return success;
}
SDL_Texture* loadTexture(char path[])
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path);
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
	}
	else
	{
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
		}

		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

int main(int argc,char * argv[])
{
   srand(time(NULL));
   int ToFast = 0;
   int XPOStmp[100];
   int YPOStmp[100];

if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
           if( !loadMedia() )
           {
             printf( "Failed to load media!\n" );
           }
           else
           {
            int i;
            int nrofAi = 0;
            for(i=0; i<rand()%20+5; i++)
            {
                aiArray[i]=createAI(rand()%1000, rand()%400);
                nrofAi++;
            }


			bool quit = false;

			SDL_Event e;
            SDL_Rect srcRect;
			while( !quit )
			{

                int ms = SDL_GetTicks();
                int sprite = ms /150 %3;
                for(i=0; i<nrofAi; i++)
                {
                    XPOStmp[i]=getAIPositionX(aiArray[i]);
                    YPOStmp[i]=getAIPositionY(aiArray[i]);
                }

				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}
                for(i=0; i<nrofAi; i++)
                {
                    //aiArray[i].Rect = {getAIPositionX(aiArray[i]),getAIPositionY(aiArray[i]),30,30};
                    SDL_Rect tmp = {getAIPositionX(aiArray[i]),getAIPositionY(aiArray[i]),30,30};
                    aiRect[i] = tmp;
                    //aiArray[i]->Rect = tmp;

                    //setRectanglePositions(aiArray[i], getAIPositionX(aiArray[i]), getAIPositionY(aiArray[i]), 30,30);
                }
                if(SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255)!=0)
                {
                    printf("RenderDrawColor fail1 %s\n",SDL_GetError());
                }
                if(SDL_RenderClear(gRenderer)!=0)
                {
                    printf("RenderClear fail %s\n",SDL_GetError());
                }

                if(SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255)!=0)
                {
                    printf("RenderDrawColor fail2 %s\n",SDL_GetError());
                }
                if(ToFast==20)
                {
                    for(i=0; i<nrofAi; i++)
                    {
                        AITick(aiArray[i]);
                    }
                    ToFast = 0;
                }
                ToFast++;
                for(i=0; i<nrofAi; i++)
                {

                    if(getAIPositionX(aiArray[i])>XPOStmp[i])
                       {
                           srcRect.x = sprite*32;
                           srcRect.y = 64;
                           srcRect.w=32;
                           srcRect.h=32;
                           printf("Going right");

                       }
                    else if(getAIPositionX(aiArray[i])<XPOStmp[i])
                       {
                           srcRect.x = sprite*32;
                           srcRect.y = 32;
                           srcRect.w=32;
                           srcRect.h=32;
                           printf("Going left");
                       }
                    else if(getAIPositionY(aiArray[i])>YPOStmp[i])
                       {
                           srcRect.x = sprite*32;
                           srcRect.y = 0;
                           srcRect.w=32;
                           srcRect.h=32;
                           printf("Going down");

                       }
                    else if(getAIPositionY(aiArray[i])<YPOStmp[i])
                       {
                           srcRect.x = sprite*32;
                           srcRect.y = 96;
                           srcRect.w=32;
                           srcRect.h=32;
                           printf("Going up");
                       }
                    SDL_RenderCopy( gRenderer, gTexture, &srcRect, &aiRect[i]);
                }

                SDL_RenderPresent( gRenderer );

			}
        }


	}
	close();

    return 0;
}
