#include "processEvents.h"

int getmPosX(GameState *game)
{
	return game->Entity.mPosX;
}
int getmPosY(GameState *game)
{
	return game->Entity.mPosY;
}

int processEvents(SDL_Window *window, GameState *game, SDL_Rect camera)
{
	SDL_Event event;
	int done = 0;
	static int lastHpTick = 0;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			if (window)
			{
				SDL_DestroyWindow(window);
				window = NULL;
				done = 1;
			}
		}
		break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
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

	ControlPlayer(game);
	PlayerWallCollision(game);

	if (lastHpTick != game->Entity.hpData.currentHp)
		UpdateHealthbar(game);
	lastHpTick = game->Entity.hpData.currentHp;
	if (camera.x < camera.w) {
		game->Entity.hpData.healthBarMax.w = game->Entity.hpData.sizeOfHealthbar;
		game->Entity.hpData.healthBarMax.x = game->Entity.mPosX;
		game->Entity.hpData.healthBarMax.y = game->Entity.mPosY - 16;
		game->Entity.hpData.healthBarCurrent.x = game->Entity.mPosX;
		game->Entity.hpData.healthBarCurrent.y = game->Entity.mPosY - 16;
	}

	//printf("manx: %d\n",game->Entity.mPosX );
	//printf("many: %d\n",game->Entity.mPosY );

	return done;

}
