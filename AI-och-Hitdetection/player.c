#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "playField.h"
#include "gameStruct.h"
#include "loadMedia.h"
#include "processEvents.h"
#include "player.h"
#include "collision.h"

void UpdateHealthbar(GameState *game){
    int sizeOfGreenbar;
    float currentHpPercent;
    currentHpPercent = (game->Entity.hpData.currentHp)/(game->Entity.hpData.maxHp); // Hur många procent av max hpt man har för tillfället
    sizeOfGreenbar = currentHpPercent*game->Entity.hpData.sizeOfHealthbar; // hur stor yta av hela healthbaren procenten motsvarar
    game->Entity.hpData.healthBarCurrent.w = sizeOfGreenbar; //sätter storleken av den gröna baren till samma storlek den bör vara
    game->Entity.hpData.healthBarMax.w = game->Entity.hpData.sizeOfHealthbar - sizeOfGreenbar; //sätter storleken av den röda baren till samma storlek den bör vara
}

void spawnAttack(Entity *player) {
    player->attack = player->rect;

    player->attack.x -= TILESIZE /2;
    player->attack.y -= TILESIZE /2;
    player->attack.w += TILESIZE /2;
    player->attack.h += TILESIZE /2;
}


void ControlPlayer(GameState *game, long ticks){
    const Uint8 *state = SDL_GetKeyboardState(NULL); //Flyttar på spelaren om det inte finns hinder(Collision detection)

    if(state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]){ // Spelaren går till vänster
        spriteFacing.x = 32;
        spriteFacing.y = 32;
        game->Entity.rect.x -= PLAYER_SPEED;
        if(collisionDetection(&game->Entity)) {
           game->Entity.rect.x += PLAYER_SPEED;
        }
    }

    if(state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]){ // Spelaren går till höger
        spriteFacing.x = 0;
        spriteFacing.y = 32;
        game->Entity.rect.x += PLAYER_SPEED;
        if(collisionDetection(&game->Entity)){
           game->Entity.rect.x -= PLAYER_SPEED;
        }
    }

    if(state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]){ // Spelaren går upp
        spriteFacing.x = 0;
        spriteFacing.y = 0;
        game->Entity.rect.y -= PLAYER_SPEED;
        if(collisionDetection(&game->Entity)){
           game->Entity.rect.y += PLAYER_SPEED;
        }
    }

    if(state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]){ // Spelaren går ner
        spriteFacing.x = 32;
        spriteFacing.y = 0;
        game->Entity.rect.y += PLAYER_SPEED;
        if(collisionDetection(&game->Entity)){
           game->Entity.rect.y -= PLAYER_SPEED;
        }
    }

    if(state[SDL_SCANCODE_SPACE]){ // Spelaren går Attackerar
        if(game->Entity.attackCD == 0 || (ticks - game->Entity.attackCD) >10){
            spawnAttack(&game->Entity);
            if(collisionDetection(&game->Entity)){
               game->Entity.rect.y -= PLAYER_SPEED;
            }
            game->Entity.attackCD = ticks;
        }

    }
}


