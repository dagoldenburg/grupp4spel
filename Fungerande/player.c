#include "player.h"

void UpdateHealthbar(GameState *game) {
	int sizeOfGreenbar;
	float currentHpPercent;
	currentHpPercent = (game->Entity.hpData.currentHp) / (game->Entity.hpData.maxHp); // Hur m�nga procent av max hpt man har f�r tillf�llet
	sizeOfGreenbar = currentHpPercent*game->Entity.hpData.sizeOfHealthbar; // hur stor yta av hela healthbaren procenten motsvarar
	game->Entity.hpData.healthBarCurrent.w = sizeOfGreenbar; //s�tter storleken av den gr�na baren till samma storlek den b�r vara
	game->Entity.hpData.healthBarMax.w = game->Entity.hpData.sizeOfHealthbar - sizeOfGreenbar; //s�tter storleken av den r�da baren till samma storlek den b�r vara
}

void ControlPlayer(GameState *game) {
	const Uint8 *state = SDL_GetKeyboardState(NULL); //Flyttar p� spelaren om det inte finns hinder(Collision detection)

	if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) { // Spelaren g�r till v�nster
		spriteFacing.x = 32;
		spriteFacing.y = 32;
		game->Entity.mPosX -= PLAYER_SPEED;
		if (collisionDetection(&game->Entity)) {
			game->Entity.mPosX += PLAYER_SPEED;
		}
	}

	if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) { // Spelaren g�r till h�ger
		spriteFacing.x = 0;
		spriteFacing.y = 32;
		game->Entity.mPosX += PLAYER_SPEED;
		if (collisionDetection(&game->Entity)) {
			game->Entity.mPosX -= PLAYER_SPEED;
		}
	}

	if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) { // Spelaren g�r upp
		spriteFacing.x = 0;
		spriteFacing.y = 0;
		game->Entity.mPosY -= PLAYER_SPEED;
		if (collisionDetection(&game->Entity)) {
			game->Entity.mPosY += PLAYER_SPEED;
		}
	}

	if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) { // Spelaren g�r ner
		spriteFacing.x = 32;
		spriteFacing.y = 0;
		game->Entity.mPosY += PLAYER_SPEED;
		if (collisionDetection(&game->Entity)) {
			game->Entity.mPosY -= PLAYER_SPEED;
		}
	}
}
