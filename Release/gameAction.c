#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>


#elif __linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#endif
#include "gameStruct.h"
#include "main.h"
#include "playField.h"
#include "gameAction.h"
#include "gameObject.h"
int getmPosX(Entity *entity)
{
    return entity->object.rect.x;
}
int getmPosY(Entity *entity)
{
    return entity->object.rect.y;
}




void updatePlayerMovement(GameState *gamestate){
        /*
        Funktionen skickar iväg spelarens position till servern som sedan skickar iväg till alla klienter
        */
        char string[30]; //Strängen som ska skickas till servern
        int x,y;
        char xArr[4],yArr[4], facing; //Char strängen där X och Y koordinaterna ska converteras och sedan läggas in i strängen som ska skickas iväg
        sprintf(string,"movplyr, ID:00%d x:0000 y:0000.",gamestate->mySlot); //Strukturen för strängen som ska skickas iväg, movplyr förklarar vilken typ av sträng det är.
        //Här läggs även till spelarens ID (gamestate->mySlot)

        x = getmPosX(&gamestate->playerEntity[gamestate->mySlot]); //Få positionen på spelaren
        stringManipulate(x,21,string);//Convertera och lägg in den i strängen vid positionen 21

        y = getmPosY(&gamestate->playerEntity[gamestate->mySlot]);
        stringManipulate(y,28,string);//Convertera och lägg in den i strängen vid positionen 28

        string[29] = gamestate->playerEntity[gamestate->mySlot].facing + '0';//Lägg in vilket håll spelaren kollar mot i strängen.

        safeSend(string,gamestate); //Skicka iväg
    return;
}

/*void sendName(GameState *gamestate){
        char string[30];
        sprintf(string,"plyrnam, ID:00%d N:",gamestate->mySlot);
        for(int i = 0; i<12; i++){
            string[18 + i] = gamestate->playerEntity[gamestate->mySlot].name[i];
        }
        safeSend(string,gamestate);
    return;
}*/



void giveDamage(GameState *gamestate,int i){
    /*
    Funktionen uppdaterar "Healthpoints" på Ains som har blivit slagna av spelaren och skickar iväg det till server
    som sedan skickar den till alla klienter.
    */
    char string[30];
    int hp,id;
    strcpy(string,"givedmg, ID:000 hp:0000......"); //Strukturen på strängen, givedmg förklarar vilken typ av sträng det är så klienterna förstår
    gamestate->AiEntity[i].hpData.currentHp -= gamestate->playerEntity[gamestate->mySlot].strength; //Sänker Ains HP
    hp = gamestate->AiEntity[i].hpData.currentHp;
    stringManipulate(i,14,string); //Lägger in Ains ID som hade blivit attackerad i strängen
    stringManipulate(hp,22,string);//Lägger in Ains HP i strängen.
    safeSend(string,gamestate); //Skicka iväg strängen till servern
return;

}

void sendCharacterType(GameState *gamestate){
    /*
    Funktionen skickar iväg spelarens Charaktär om han/hon är en Fighter eller mage.
    */
    char string[30];
    int id = gamestate->mySlot;
    int type = gamestate->playerEntity[gamestate->mySlot].characterType; //Charaktären
    strcpy(string,"chartyp, ID:000 Typ:0000....."); //Strukturen på strängen, chartyp förklarar vilken typ av sträng det är så klienterna förstår
    stringManipulate(id,14,string); //Lägger in spelarens ID i strängen
    stringManipulate(type,23,string);//Lägger in Charaktären i strängen
    safeSend(string,gamestate); //Skicka iväg strängen
return;

}


void spawnAttack(Entity *playerEntity) {
    /*
    Funktionen skapar ett attack rektangel baserat på om spelaren är en Fighter eller Mage och åt vilket håll spelaren tittar mot.
    Fighter attackerar med svärden och därför är inte en projektil attack, då behöver sätta attack.Vel till noll.
    Mage däremot har projektil attack och attack.vel beskriver hur snabb projektilen ska vara.
    spriteFacing beskriver vilket håll spelaren tittar mot, 0 = UPP, 32 = NER, 96 = Höger,  64 = Vänster.
    */
    playerEntity->attack.rect = playerEntity->object.rect;

    if(playerEntity->characterType == 0) {
        if(playerEntity->spriteFacing.y == 0){ ///Up
        playerEntity->attack.rect.x -= TILESIZE/4;
        playerEntity->attack.rect.w += TILESIZE/2;
        playerEntity->attack.rect.y -= TILESIZE;

        playerEntity->attack.velX = 0;
        playerEntity->attack.velY = 0;
    }

    if(playerEntity->spriteFacing.y == 32){ ///Down
        playerEntity->attack.rect.x -= TILESIZE/4;
        playerEntity->attack.rect.w += TILESIZE/2;
        playerEntity->attack.rect.y += TILESIZE;


        playerEntity->attack.velX = 0;
        playerEntity->attack.velY = 0;
    }

    if(playerEntity->spriteFacing.y == 96){ ///Right
        playerEntity->attack.rect.x += TILESIZE;
        playerEntity->attack.rect.h += TILESIZE/2;
        playerEntity->attack.rect.y -= TILESIZE/4;

        playerEntity->attack.velX = 0;
        playerEntity->attack.velY = 0;
    }

    if(playerEntity->spriteFacing.y == 64){ ///Left
        playerEntity->attack.rect.x -= TILESIZE;
        playerEntity->attack.rect.h += TILESIZE/2;
        playerEntity->attack.rect.y -= TILESIZE/4;

        playerEntity->attack.velX = 0;
        playerEntity->attack.velY = 0;
    }

}

if(playerEntity->characterType == 1) {
    if(playerEntity->spriteFacing.y == 0){ ///Up
        playerEntity->attack.rect.x += TILESIZE/5;
        playerEntity->attack.rect.w = TILESIZE/2;
        playerEntity->attack.rect.y -= TILESIZE;

        playerEntity->attack.spriteFacing.x = 40;
        playerEntity->attack.spriteFacing.h = 16;
        playerEntity->attack.spriteFacing.w = 8;

        playerEntity->attack.velX = 0;
        playerEntity->attack.velY = -(playerEntity->strength * 2);
    }

    if(playerEntity->spriteFacing.y == 32){ ///Down
        playerEntity->attack.rect.x += TILESIZE/5;
        playerEntity->attack.rect.w = TILESIZE/2;
        playerEntity->attack.rect.y += TILESIZE;

        playerEntity->attack.spriteFacing.x = 32;
        playerEntity->attack.spriteFacing.h = 16;
        playerEntity->attack.spriteFacing.w = 8;

        playerEntity->attack.velX = 0;
        playerEntity->attack.velY = (playerEntity->strength * 2);
    }

    if(playerEntity->spriteFacing.y == 96){ ///Right
        playerEntity->attack.rect.x += TILESIZE;
        playerEntity->attack.rect.h = TILESIZE/2;
        playerEntity->attack.rect.y += TILESIZE/4;

        playerEntity->attack.spriteFacing.x = 0;
        playerEntity->attack.spriteFacing.h = 8;
        playerEntity->attack.spriteFacing.w = 16;

        playerEntity->attack.velX = (playerEntity->strength * 2);
        playerEntity->attack.velY = 0;
    }

    if(playerEntity->spriteFacing.y == 64){ ///Left
        playerEntity->attack.rect.x -= TILESIZE;
        playerEntity->attack.rect.h = TILESIZE/2;
        playerEntity->attack.rect.y += TILESIZE/4;

        playerEntity->attack.spriteFacing.x = 16;
        playerEntity->attack.spriteFacing.h = 8;
        playerEntity->attack.spriteFacing.w = 16;

        playerEntity->attack.velX = -(playerEntity->strength * 2);
        playerEntity->attack.velY = 0;
    }

}
return;
}

void attackTick(Attack *attack) {
    /*
    Funktionen gör så att projektil attacker förflyttas.
    */
    if(attack->velX != 0 || attack->velY != 0) {///Kontrollerar så att attacken verkligen är en projektil
    attack->rect.x += attack->velX;
    attack->rect.y += attack->velY;
    }

}

int attackHandle(Attack *attack){
    /*
    Kontrollerar så att attacken inte hamnar utanför spelplanen och resettar attacken om den nuddar en Kollisions TILE.
    Funktionen är endast till för projektila attacker.
    */
    if(attack->velX != 0 || attack->velY != 0) { ///Kontrollerar så att attacken verkligen är en projektil
        if(TilemapCollisionDetection(attack->rect)){//Kollision med Tile
            return 1;
        }else if(attackWallCollision(attack)){//Kollision med kanterna av spelplanen
            return 1;
        }
    }
    return 0;
}


int attackWallCollision(Attack *attack)
/*
    Funktionen tar han om attacker som hamnar utanför spelplanen. Returnar 1 om det har hänt kollsion och 0 om det inte.
*/
{


    if( attack->rect.x<0 ) ///Vänstra Kanten
    {
       return 1;
    }
    if( attack->rect.x+20>LEVEL_WIDTH ) ///Högra Kanten
    {
        return 1;
    }

    if( attack->rect.y<0  ) ///Övre kanten
    {
        return 1;
    }
    if( attack->rect.y+20>LEVEL_HEIGHT )///Nedre Kanten
    {
        return 1;
    }

    return 0;
}

void controlplayer(GameState *gamestate)
{
    /*
    Funktionen tar hand om alla input kommand från användaren. Antingen handlar det om förflyttning på spelaren eller attack.
    Redan i denna funktion väljs det vilken sprite som ska användas beroende på vilket håll spelaren kollar mot.
    Med SDL_Getticks kan man göra en animation så det ser ut som att han går eller attackerar.
    */
    int ms = SDL_GetTicks();
    int sprite = ms /3 % 3;
    int tmpFaceing = gamestate->playerEntity[gamestate->mySlot].facing; ///Kommer ihåg vilket håll spelaren kollade mot från början
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_LEFT]) ///Vänster
    {
        gamestate->playerEntity[gamestate->mySlot].spriteFacing.x=sprite*32; ///Gör så att det blir animering
        gamestate->playerEntity[gamestate->mySlot].spriteFacing.y=64; ///Vart datorn kommer att kolla i sprite-sheeten
        gamestate->playerEntity[gamestate->mySlot].facing = 3; ///Vilket håll spelaren kollar mot

        gamestate->playerEntity[gamestate->mySlot].object.rect.x -= PLAYER_SPEED; ///Uppdaterar spelarens position
        if(TilemapCollisionDetection(gamestate->playerEntity[gamestate->mySlot].object.rect))///Kollar om spelaren hamnat in i en kollisionstile
        {
           gamestate->playerEntity[gamestate->mySlot].object.rect.x += PLAYER_SPEED; ///Om det ovanför blir sann så återställer den positionen
        }
    }
    if(state[SDL_SCANCODE_RIGHT]) ///Höger
    {

        gamestate->playerEntity[gamestate->mySlot].spriteFacing.x=sprite*32;
        gamestate->playerEntity[gamestate->mySlot].spriteFacing.y=96;
        gamestate->playerEntity[gamestate->mySlot].facing = 1;

        gamestate->playerEntity[gamestate->mySlot].object.rect.x += PLAYER_SPEED;
        if(TilemapCollisionDetection(gamestate->playerEntity[gamestate->mySlot].object.rect))
        {
           gamestate->playerEntity[gamestate->mySlot].object.rect.x -= PLAYER_SPEED;
        }
    }
    if(state[SDL_SCANCODE_UP])
    {
        gamestate->playerEntity[gamestate->mySlot].spriteFacing.x=sprite*32;
        gamestate->playerEntity[gamestate->mySlot].spriteFacing.y=0;
        gamestate->playerEntity[gamestate->mySlot].facing = 0;

        gamestate->playerEntity[gamestate->mySlot].object.rect.y -= PLAYER_SPEED;
        if(TilemapCollisionDetection(gamestate->playerEntity[gamestate->mySlot].object.rect))
        {
          gamestate->playerEntity[gamestate->mySlot].object.rect.y += PLAYER_SPEED;
        }
    }
    if(state[SDL_SCANCODE_DOWN])
    {
        gamestate->playerEntity[gamestate->mySlot].spriteFacing.x=sprite*32;
        gamestate->playerEntity[gamestate->mySlot].spriteFacing.y=32;
        gamestate->playerEntity[gamestate->mySlot].facing = 2;
        gamestate->playerEntity[gamestate->mySlot].object.rect.y += PLAYER_SPEED;
        if(TilemapCollisionDetection(gamestate->playerEntity[gamestate->mySlot].object.rect))
        {
           gamestate->playerEntity[gamestate->mySlot].object.rect.y -= PLAYER_SPEED;
        }
    }
    if(state[SDL_SCANCODE_SPACE]){///När användaren vill attackera
        if(gamestate->playerEntity[gamestate->mySlot].attackTimer == 0 || (SDL_GetTicks() - gamestate->playerEntity[gamestate->mySlot].attackTimer) >100){///Delay för att attackeera så det finns en mellanrum
            if(gamestate->playerEntity[gamestate->mySlot].attack.rect.x == 0) { ///Kontrollerar om en förgående attack existerar
                spawnAttack(&gamestate->playerEntity[gamestate->mySlot]);///Spawnar attacken
                sendAttack(gamestate);///Skickar iväg till alla klienter att det skapats en attack.
                gamestate->playerEntity[gamestate->mySlot].attackTimer = SDL_GetTicks();///Starta delayen
            }
        }
    }

    if(gamestate->playerEntity[gamestate->mySlot].facing == tmpFaceing && gamestate->playerEntity[gamestate->mySlot].facing!=4) {///Om inget har hänt står så tillsätter den facing = 4 vilket animerar så spelaren står stilla
        gamestate->playerEntity[gamestate->mySlot].facing = 4;
        updatePlayerMovement(gamestate);
    }
    playWallCollision(&gamestate->playerEntity[gamestate->mySlot].object);
        ///*********************player colliction detector with wall*************************////

}

int checkIFObjectHit(SDL_Rect *playerRect, SDL_Rect *AiRect){
    /*
    Funktionen tar hand om kollision mellan två objekter, det kan vara från attacker till entiteter.
    */
    if (playerRect->x <= AiRect->x + AiRect->w&&
       playerRect->x + playerRect->w >= AiRect->x &&
       playerRect->y <= AiRect->y + AiRect->h &&
       playerRect->y + playerRect->w >= AiRect->y) {
        return 1;
    }
    return 0;
}

void sendAttack(GameState *gamestate) {
    /*
    Skickar iväg den spawnade attacken till alla klienter
    */
    char string2[30];
    int id;
    strcpy(string2,"spwnatt, id:000..............");//Strukturen på strängen
    id = gamestate->mySlot;//Spelarens ID

    stringManipulate(id,14,string2);//Lägger in spelarens id i strängen
    printf("trying to send player attack\n");
    safeSend(string2,gamestate);//Skicka iväg till alla
}

void resetAttack(Attack *attack){
    /*
    Nollställer attacken
    */
    attack->rect.h = 0;
    attack->rect.w = 0;
    attack->rect.x = 0;
    attack->rect.y = 0;
    attack->velX = 0;
    attack->velY = 0;

}


void sendResetAttack(GameState *gamestate){
    /*
    Skicka iväg till alla klienter att spelarens attack har resettas.
    */
    char string[30];
    int id;

    strcpy(string,"resetAT, ID:000..............");
    id = gamestate->mySlot;
    stringManipulate(id,14,string);
    safeSend(string,gamestate);
}

void playWallCollision(gameObject *object)
/*
    Funktionen tar hand om objekter som hamnar utanför spelplanen och återställer deras koordinater.
*/
{


    if( object->rect.x<0 )
    {
       object->rect.x=0;
    }
    if( object->rect.x+20>LEVEL_WIDTH )
    {

        object->rect.x=LEVEL_WIDTH-20;
    }

    if( object->rect.y<0  )
    {
       object->rect.y=0;
    }
    if( object->rect.y+20>LEVEL_HEIGHT )
    {
        object->rect.y=LEVEL_HEIGHT-20;
    }

}
int TilemapCollisionDetection(SDL_Rect rect){
    /*
        Funktionen tar hand om objekter som hamnar i kollisions tile och returnar 1 om det sant annars returnar 0

    */
    int LEFT = rect.x;//Lättare att förstå
    int TOP = rect.y;
    int RIGHT = rect.x + rect.w;
    int BOTTOM = rect.y + rect.h;

     for(int i = 0; i<TOTAL_COLLISION_TILES; i++) {
        if(playfield[(TOP + PLAYER_SPEED)/TILESIZE][(LEFT + PLAYER_SPEED)/TILESIZE]==collisionTiles[i]) //COLLISION MED ÖVRE VÄNSTER KANT
        {
            return 1;
        }
        if(playfield[(BOTTOM - PLAYER_SPEED)/TILESIZE][(RIGHT - PLAYER_SPEED)/TILESIZE]==collisionTiles[i])//COLLISION MED NEDRE HÖGER KANT
        {
            return 1;
        }

        if(playfield[(TOP + PLAYER_SPEED)/TILESIZE][(RIGHT - PLAYER_SPEED)/TILESIZE]==collisionTiles[i])//COLLISION MED ÖVRE HÖGER KANT
        {
            return 1;
        }

        if(playfield[(BOTTOM - PLAYER_SPEED)/TILESIZE][(LEFT + PLAYER_SPEED)/TILESIZE]==collisionTiles[i]) //COLLISION MED NEDRE VÄNSTER KANT
        {
            return 1;
        }
    }
    return 0;
}
int processEvents(SDL_Window *window, GameState *game)
/*
    Funktionen tar hand om alla inputs som användaren gör t.ex stänga av spelen eller flytta charäktären i spelplanen.
*/
{

    SDL_Event event;
    int done = 0;

    while(SDL_PollEvent(&event))///Kollar om spelaren vill stänga av spelet
    {
    switch(event.type)
    {
      case SDL_WINDOWEVENT_CLOSE:
      {
        if(window)
        {
          SDL_DestroyWindow(window);
          window = NULL;
          done = 1;
        }
      }
      break;
      case SDL_KEYDOWN:
      {
        switch(event.key.keysym.sym)
        {
          case SDLK_ESCAPE:
            done = 1;
          break;
        }
      }
      break;
      case SDL_QUIT:
        //quit out of the game
        done = 1;
      break;
    }
    }

    controlplayer(game);///Player action

  return done;

}

