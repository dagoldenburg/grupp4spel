#ifndef GAMEACTION_H_INCLUDED
#define GAMEACTION_H_INCLUDED

int getmPosX(Entity *entity);
int getmPosY(Entity *entity);
void controlplayer(Entity *Entity);
void playWallCollision(gameObject *object);
int TilemapCollisionDetection(SDL_Rect rect);

int processEvents(SDL_Window *window, GameState *game);


#endif // GAMEACTION_H_INCLUDED
