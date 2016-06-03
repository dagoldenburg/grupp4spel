#ifndef GAMEACTION_H_INCLUDED
#define GAMEACTION_H_INCLUDED

int getmPosX(Entity *entity);
int getmPosY(Entity *entity);
void controlplayer(GameState *gamestate);
void playWallCollision(gameObject *object);
int TilemapCollisionDetection(SDL_Rect rect);
void sendCharacterType(GameState *gamestate);
void resetAttack(Attack *attack);
int attackHandle(Attack *attack);
int attackWallCollision(Attack *attack);
void attackTick(Attack *attack);
void sendAttack(GameState *gamestate);
void sendResetAttack(GameState *gamestate);
void sendName(GameState *gamestate);
int processEvents(SDL_Window *window, GameState *game);


#endif // GAMEACTION_H_INCLUDED
