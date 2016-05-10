#ifndef GAMEACTION_H_INCLUDED
#define GAMEACTION_H_INCLUDED

int getmPosX(Entity *entity);
int getmPosY(Entity *entity);
void controlplayer(Entity *Entity);
void playWallCollision(gameObject *object);
int TilemapCollisionDetection(SDL_Rect rect);
void spawnAttack(Entity *playerEntity);
int checkIFAttackHit(SDL_Rect playerRect, SDL_Rect AiRect);
int checkIFAiDead(Entity *player, Entity *AI);
void resetAttack(SDL_Rect *attack);
int processEvents(SDL_Window *window, GameState *game);


#endif // GAMEACTION_H_INCLUDED
