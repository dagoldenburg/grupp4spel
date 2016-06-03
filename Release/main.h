#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>


#elif __linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#endif

#define LEVEL_WIDTH 1920
#define LEVEL_HEIGHT 1088
//Screen dimension constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640
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

extern int threadJoinQuerry;

extern SDL_Rect spriteFacing;

#endif // MAIN_H_INCLUDED
