#include "collision.h"

void PlayerWallCollision(GameState *game) {

	if (game->Entity.mPosX<0) { // Collision med vänstra delen av rutan
		game->Entity.mPosX = 0;
	}
	if (game->Entity.mPosX + TILESIZE + PLAYER_SPEED>LEVEL_WIDTH) { // Collision med högra delen av rutan
		game->Entity.mPosX = LEVEL_WIDTH - TILESIZE - PLAYER_SPEED;
	}

	if (game->Entity.mPosY<0) { // Collision med övre delen av  rutan
		game->Entity.mPosY = 0;
	}
	if (game->Entity.mPosY + TILESIZE + PLAYER_SPEED>LEVEL_HEIGHT) { // Collision med nedre delen av rutan
		game->Entity.mPosY = LEVEL_HEIGHT - TILESIZE - PLAYER_SPEED;
	}

}

int collisionDetection(Entity *object) {
	for (int i = 0; i<TOTAL_COLLISION_TILES; i++) {
		if (playfield[(object->mPosY + 5) / TILESIZE][(object->mPosX + 5) / TILESIZE] == collisionTiles[i]) //Kontrollerar om det finns ett hinder, annars flyttar objektet
		{
			printf("Upper box\n");
			return 1;
		}
		if (playfield[(object->mPosY + TILESIZE - 5) / TILESIZE][(object->mPosX + TILESIZE - 5) / TILESIZE] == collisionTiles[i])
		{
			printf("Lower box\n");
			return 1;
		}

		if (playfield[(object->mPosY + 5) / TILESIZE][(object->mPosX + TILESIZE - 5) / TILESIZE] == collisionTiles[i])
		{
			printf("Right box\n");
			return 1;
		}

		if (playfield[(object->mPosY + TILESIZE - 5) / TILESIZE][(object->mPosX + 5) / TILESIZE] == collisionTiles[i])
		{
			printf("Left box\n");
			return 1;
		}
	}
	return 0;
}