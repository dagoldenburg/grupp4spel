#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#include "init.h"
#include "playField.h"
#include "gameStruct.h"
#include "loadMedia.h"
#include "processEvents.h"
#include "player.h"
#include "collision.h"

#define PLAYER_SPEED 2

void ControlPlayer(GameState *game);

void UpdateHealthbar(GameState *game);

#endif // PLAYER_H_INCLUDED