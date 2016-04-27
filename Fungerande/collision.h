#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#include "init.h"
#include "playField.h"
#include "gameStruct.h"
#include "loadMedia.h"
#include "processEvents.h"
#include "player.h"

int collisionDetection(Entity *object);
void PlayerWallCollision(GameState *game);

#endif // COLLISION_H_INCLUDED