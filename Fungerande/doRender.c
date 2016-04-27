#include "doRender.h"

void doRender(SDL_Renderer *renderer, GameState *game, SDL_Rect mCam)
{
	//set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	//Clear the screen (to blue)
	SDL_RenderClear(renderer);
	for (int i = 0; i<COLUMNS; i++)
	{
		for (int j = 0; j<ROWS; j++)
		{
			if (playfield[i][j] == 0)
			{
				SDL_Rect camera2 = { j*TILESIZE - mCam.x,i*TILESIZE - mCam.y,TILESIZE,TILESIZE };
				SDL_RenderCopy(renderer, game->gTileTexture.mTexture, &game->TileClip[0], &camera2);
			}
			if (playfield[i][j] == 1)
			{
				SDL_Rect camera2 = { j*TILESIZE - mCam.x,i*TILESIZE - mCam.y,TILESIZE,TILESIZE };
				SDL_RenderCopy(renderer, game->gTileTexture.mTexture, &game->TileClip[1], &camera2);
			}
			if (playfield[i][j] == 2)
			{
				SDL_Rect camera3 = { j*TILESIZE - mCam.x,i*TILESIZE - mCam.y,TILESIZE,TILESIZE };
				SDL_RenderCopy(renderer, game->gTileTexture.mTexture, &game->TileClip[2], &camera3);
			}
			if (playfield[i][j] == 3)
			{
				SDL_Rect camera2 = { j*TILESIZE - mCam.x,i*TILESIZE - mCam.y,TILESIZE,TILESIZE };
				SDL_RenderCopy(renderer, game->gTileTexture.mTexture, &game->TileClip[3], &camera2);
			}
		}

	}

	//
	//    SDL_Rect camera={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	//    SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&mCam,&camera);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	//set the drawing color to white
	SDL_Rect rect = { game->Entity.mPosX - mCam.x,game->Entity.mPosY - mCam.y, TILESIZE, TILESIZE };

	SDL_RenderCopy(renderer, game->gPlayerTexture.mTexture, &spriteFacing, &rect);
	// SDL_RenderFillRect(renderer, &rect);

	if (SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF)) {
		printf("Could not set render draw color for healthbar max. SDL_ERROR: %s\n", SDL_GetError());
	}

	if (SDL_RenderFillRect(renderer, &game->Entity.hpData.healthBarMax)<0) {
		printf("Couldnt render fill rect healthbar max. SDL_ERROR: %s\n", SDL_GetError());
	}
	if (SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF)) {
		printf("Could not set render draw color for healthbar current. SDL_ERROR: %s\n", SDL_GetError());
	}
	if (SDL_RenderFillRect(renderer, &game->Entity.hpData.healthBarCurrent)<0) {
		printf("Couldnt render fill rect for healthbar current. SDL_ERROR: %s\n", SDL_GetError());
	}

	SDL_RenderPresent(renderer);
}