#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include"includes.h"
typedef struct game
{
	bool Running;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event Event;
}GAME;

GAME theGame;

int OnExecute();
bool OnInit();
bool LoadContent();
void OnEvent(SDL_Event* Event);
void OnLoop();
void OnRender();
void Cleanup();


#endif // GAME_H_INCLUDED
