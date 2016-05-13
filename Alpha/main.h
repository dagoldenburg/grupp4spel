#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <SDL2/SDL.h>

#define LEVEL_WIDTH 1920
#define LEVEL_HEIGHT 1088
//Screen dimension constants
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define TOTAL_TILES  192
#define TOTAL_TILE_SPRITES 12
#define TOTAL_COLLISION_TILES 11
#define PLAYER_SPEED 10
#define ROWS 60
#define COLUMNS 34
#define TILESIZE 32
#define TILE_RED 2
#define TILE_GREEN 1
#define TILE_BLACK 0
char recvbuffer[100];

extern SDL_Rect spriteFacing;

#endif // MAIN_H_INCLUDED
