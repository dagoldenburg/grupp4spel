#include "SDL2/SDL.h"
#include <stdio.h>

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 480;

#define false 0
#define true 1
#define successfull 1
#define failed 0
#define X 0
#define Y 1



int Initialize();
void Close();

struct PlayableCharacter{
    SDL_Rect playerBox; // destination rectangle, rectangeln som spelaren ska ritas i
    int xSpeed;
    int ySpeed;

};
struct SpriteSheetRects{
    SDL_Rect normalFace;
    SDL_Rect upsideDownFace;
    SDL_Rect leftFace;
    SDL_Rect rightFace;
};
    SDL_Event e; // EXIT EVENT
	SDL_Window* window = NULL; // Fönstret
    SDL_Surface* screen = NULL; // Pixel buffert (?)
    SDL_Surface* modelSurface = NULL;
    SDL_Texture* playerTexture = NULL; // Spelar spriten
    SDL_Texture* worldTexture = NULL; // Texture att rita
    SDL_Renderer* renderer = NULL; // Renderer


int main(int argc, char* args[]) {

    int exit = false; // RUN VARIABLE

    // TODO: Rensa upp minnet för player.
    struct PlayableCharacter* player = malloc(sizeof(struct PlayableCharacter));
    struct SpriteSheetRects rects;
    SDL_Rect facing = rects.normalFace;

    rects.normalFace.x = 0;
    rects.normalFace.y = 0;
    rects.normalFace.h = 32;
    rects.normalFace.w = 32;

    rects.upsideDownFace.x = 32;
    rects.upsideDownFace.y = 0;
    rects.upsideDownFace.h = 32;
    rects.upsideDownFace.w = 32;

    rects.leftFace.x = 32;
    rects.leftFace.y = 32;
    rects.leftFace.h = 32;
    rects.leftFace.w = 32;

    rects.rightFace.x = 0;
    rects.rightFace.y = 32;
    rects.rightFace.h = 32;
    rects.rightFace.w = 32;

    //Player attributes
    player->playerBox.x = SCREEN_WIDTH/2;
    player->playerBox.y = SCREEN_HEIGHT/2;
    player->playerBox.h = 32;
    player->playerBox.w = 32;
    player->ySpeed=0;
    player->xSpeed=0;

	if (!Initialize(player,rects.normalFace)) {
		printf("Could not init.\n");
		return failed;
	}

	while (!exit){ // EVENTLOOP
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) // Stöd för X knappen och Alt+f4 för avstängning
				exit = true;
            if(e.type == SDL_KEYDOWN && e.key.repeat==0) // så länge man håller ner den här så har playern velocity
                switch(e.key.keysym.sym){
                    case SDLK_w: player->ySpeed += -3; facing = rects.normalFace; break;
                    case SDLK_a: player->xSpeed += -3; facing = rects.leftFace; break;
                    case SDLK_s: player->ySpeed += 3;  facing = rects.upsideDownFace; break;
                    case SDLK_d: player->xSpeed += 3; facing = rects.rightFace; break;
                }
            else if(e.type == SDL_KEYUP && e.key.repeat==0) // när man släpper här så förvinner velocitien från playern
                switch(e.key.keysym.sym){
                    case SDLK_w: player->ySpeed += 3; break;
                    case SDLK_a: player->xSpeed += 3; break;
                    case SDLK_s: player->ySpeed += -3; break;
                    case SDLK_d: player->xSpeed += -3; break;
                }
            player->playerBox.y += player->ySpeed; // Uppdaterar positionen för  playern
            player->playerBox.x += player->xSpeed;  // Uppdaterar positionen för  playern

            SDL_RenderClear(renderer); //Rendrar om spelplanen och playern
            RenderBackground(); //Rendrar om spelplanen och playern
            RenderCharacter(player, facing); //Rendrar om spelplanen och playern
            }
		}
	Close();
	return successfull;
}

int RenderBackground(){ //rendrar bakgrunden

        if(SDL_RenderCopy(renderer, worldTexture, NULL, NULL) < 0){ //Kopierar texturn till renderern så den kan skrivar till skrämen med RenderPresent
            printf("Error copying world texture to renderer. SDL_ERROR: %s\n",SDL_GetError());
        }
        SDL_RenderPresent(renderer); // Ritar det som finns i buffern till skärmen, returnar inget värde
        return successfull;
}

int LoadAssets(){

    const char* spriteSheet = "models/spriteSheet2.bmp";
    const char* MapName = "maps/testWorld.bmp";

    if(!(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED))){ // Skapar en renderer
            printf("Unable to create renderer. SDL_ERROR: %s\n",SDL_GetError());
            return failed;
    }
    if(!(screen = SDL_LoadBMP(MapName))){ // Laddar in spelplansbilden
            printf("Unable to load %s. SDL_ERROR: %s\n", MapName, SDL_GetError());
            return failed;
    }
    if(!(worldTexture = SDL_CreateTextureFromSurface(renderer, screen))){ // Skapar en texture av det som finns i surface structen
            printf("Unable to create world texture from surface. SDL_ERROR: %s\n",SDL_GetError());
            return failed;
    }
    if(!(modelSurface = SDL_LoadBMP(spriteSheet))){ // Laddar in spelarbilden
        printf("Unable to load %s. SDL_ERROR: %s\n", spriteSheet, SDL_GetError());
        return failed;
    }
    if(SDL_SetColorKey(modelSurface,SDL_TRUE,SDL_MapRGB(modelSurface->format,0xE0,0x80,0xC0))<0){ //Sätter en colorkey som gör en viss färg transparant
        printf("Unable to set colorkey. SDL ERROR: %s\n", SDL_GetError);
        return failed;
    }
    if(!(playerTexture = SDL_CreateTextureFromSurface(renderer, modelSurface))){ // Skapar en texture av det som finns i surface structen
        printf("Unable to create player texture from surface. SDL_ERROR: %s\n",SDL_GetError());
        return failed;
    }
    return successfull;
}

int RenderCharacter(struct PlayableCharacter *player, SDL_Rect facing){

    if(SDL_RenderCopy(renderer, playerTexture, &facing, &player->playerBox) < 0){ //Kopierar texturen till renderern så den kan skrivar till skrämen med RenderPresent,
        printf("Error copying player texture to renderer. SDL_ERROR: %s\n",SDL_GetError());
    }
    SDL_RenderPresent(renderer); // Ritar det som finns i buffern till skärmen, returnar inget värde
}


int Initialize(struct PlayableCharacter *player,SDL_Rect facing) { // Initializerar sdl, window, spelartexturen och laddar in världen

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL COULD NOT BE INITIALIZED! SDL_ERROR: %s\n", SDL_GetError());
		return failed;
	}
	if(!(window = SDL_CreateWindow("SDL Spel",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH, SCREEN_HEIGHT,
                               SDL_WINDOW_SHOWN))){
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return failed;
    }

    LoadAssets();
    RenderBackground();
    RenderCharacter(player,facing);

	return successfull;
}

void Close() { // Rensar upp de resurser som användes under körning

    SDL_DestroyTexture(worldTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(modelSurface);
	SDL_DestroyWindow(window);

	SDL_Quit();
}
