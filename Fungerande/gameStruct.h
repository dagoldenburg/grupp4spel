#ifndef GAMESTRUCT_H_INCLUDED
#define GAMESTRUCT_H_INCLUDED

#include "init.h"
#define TOTAL_TILES  192

typedef struct
{
	int mWidth;
	int mHeight;
	SDL_Texture* mTexture;

}LTexture;

typedef struct {
	int sizeOfHealthbar;
	float maxHp;
	float currentHp;
	SDL_Rect healthBarCurrent;
	SDL_Rect healthBarMax;
} HitPointData;

typedef struct
{
	int mPosX;
	int mPosY;

	int mVelX;
	int mVelY;
	HitPointData hpData;
}Entity;

typedef struct
{
	int mtype;
	SDL_Rect mBox;
}Tile;



typedef struct
{
	Entity Entity;
	Tile tileSet[TOTAL_TILES];
	LTexture gPlayerTexture;
	LTexture gTileTexture;
	SDL_Rect TileClip[4];
	SDL_Renderer *renderer;


}GameState;


#endif // GAMESTRUCT_H_INCLUDED
