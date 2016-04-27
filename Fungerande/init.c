#include "init.h"

SDL_Rect spriteFacing;

void initialize()
{
	GameState gamestate;
	SDL_Window *window = NULL;                    // Declare a window
	SDL_Renderer *renderer = NULL;                // Declare a renderer
	SDL_Surface *starSurface = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2
										   //srandom((int)time(NULL));


										   //Create an application window with the following settings:
	window = SDL_CreateWindow("Game Window",                     // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		SCREEN_WIDTH,                               // width, in pixels
		SCREEN_HEIGHT,                               // height, in pixels
		0                                  // flags
	);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


	SDL_Rect camera = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//Player attributes
	gamestate.Entity.mPosX = 0;
	gamestate.Entity.mPosY = 0;
	gamestate.Entity.mVelX = 0;
	gamestate.Entity.mVelY = 0;
	gamestate.renderer = renderer;
	gamestate.Entity.hpData.maxHp = 100.0;
	gamestate.Entity.hpData.currentHp = 50.0;
	gamestate.Entity.hpData.sizeOfHealthbar = 32;
	gamestate.Entity.hpData.healthBarCurrent.h = 8;
	gamestate.Entity.hpData.healthBarMax.h = 8;


	loadMedia(&gamestate);

	// The window is open: enter program loop (see SDL_PollEvent)

	/**Event loop*/
	int done = 0;
	spriteFacing.w = 32;
	spriteFacing.h = 32;
	//Event loop
	while (!done)
	{
		//Check for events

		done = processEvents(window, &gamestate, camera);
		camera.x = (getmPosX(&gamestate) + 20 / 2) - (SCREEN_WIDTH / 2);
		camera.y = (getmPosY(&gamestate) + 20 / 2) - (SCREEN_HEIGHT / 2);
		if (camera.x < 0) {
			camera.x = 0;
		}
		if (camera.y < 0) {
			camera.y = 0;
		}
		if (camera.x > LEVEL_WIDTH - camera.w) {
			camera.x = LEVEL_WIDTH - camera.w;
		}
		if (camera.y > LEVEL_HEIGHT - camera.h) {
			camera.y = LEVEL_HEIGHT - camera.h;
		}

		//Render display
		doRender(renderer, &gamestate, camera);

		//don't burn up the CPU
		SDL_Delay(10);
	}


	// Close and destroy the window
	SDL_DestroyTexture(gamestate.gTileTexture.mTexture);
	SDL_DestroyTexture(gamestate.gPlayerTexture.mTexture);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	// Clean up
	SDL_Quit();
	return 0;
}