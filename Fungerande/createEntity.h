#ifndef CREATEENTITY_H_INCLUDED
#define CREATEENTITY_H_INCLUDED
GameState createEntity(GameState entity, int x, int y, SDL_Renderer *renderer);
int getAIPositionX(GameState *entity);
int getAIPositionY(GameState *entity);

#endif // CREATEENTITY_H_INCLUDED
