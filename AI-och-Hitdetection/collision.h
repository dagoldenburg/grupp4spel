#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

int collisionDetection(Entity *object);
void PlayerWallCollision(GameState *game);
int attackCollision(Entity *player, Entity *AI);

#endif // COLLISION_H_INCLUDED
