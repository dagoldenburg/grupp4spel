#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


#elif __linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#endif

#include "gameGraphics.h"
#include "gameStruct.h"
#include "playField.h"
#include "gameNet.h"
#include "main.h"

SDL_Texture* loadImage(GameState *g,int readyToggle,SDL_Renderer *renderer){ // laddar in en bild som man har som bakgrund, används vid lobby
    SDL_Surface *lobbyMenuSurface = NULL;
    SDL_Texture *lobbyMenuTexture = NULL;
    if(g->playerEntity[g->mySlot].characterType == 0){
        if(readyToggle==0){
        printf("hej\n");
            lobbyMenuSurface = IMG_Load("textures/LobbyWarrSelectReady.png");
            if(lobbyMenuSurface==NULL){
                printf("Loss menu surface failed to load. SDL_ERROR: %s\n",SDL_GetError());
            }
            lobbyMenuTexture = SDL_CreateTextureFromSurface(renderer,lobbyMenuSurface);
            if(lobbyMenuTexture==NULL){
                printf("Loss menu surface failed to load. SDL_ERROR: %s\n",SDL_GetError());
            }
        }
        if(readyToggle==1){
        printf("hej\n");
            lobbyMenuSurface = IMG_Load("textures/LobbyWarrSelectNReady.png");
            if(lobbyMenuSurface==NULL){
                printf("Loss menu surface failed to load. SDL_ERROR: %s\n",SDL_GetError());
            }
            lobbyMenuTexture = SDL_CreateTextureFromSurface(renderer,lobbyMenuSurface);
            if(lobbyMenuTexture==NULL){
                printf("Loss menu surface failed to load. SDL_ERROR: %s\n",SDL_GetError());
            }
        }
    }
    else if(g->playerEntity[g->mySlot].characterType == 1){

        if(readyToggle==0){
        printf("hej\n");
            lobbyMenuSurface = IMG_Load("textures/LobbyMageSelectReady.png");
            if(lobbyMenuSurface==NULL){
                printf("Loss menu surface failed to load. SDL_ERROR: %s\n",SDL_GetError());
            }
            lobbyMenuTexture = SDL_CreateTextureFromSurface(renderer,lobbyMenuSurface);
            if(lobbyMenuTexture==NULL){
                printf("Loss menu surface failed to load. SDL_ERROR: %s\n",SDL_GetError());
            }
        }
        if(readyToggle==1){
        printf("hej\n");
            lobbyMenuSurface = IMG_Load("textures/LobbyMageSelectNReady.png");
            if(lobbyMenuSurface==NULL){
                printf("Loss menu surface failed to load. SDL_ERROR: %s\n",SDL_GetError());
            }
            lobbyMenuTexture = SDL_CreateTextureFromSurface(renderer,lobbyMenuSurface);
            if(lobbyMenuTexture==NULL){
                printf("Loss menu surface failed to load. SDL_ERROR: %s\n",SDL_GetError());
            }
        }
    }
    return lobbyMenuTexture;
}

void loadMedia(GameState *game) // Laddar media
{
    int i;
     SDL_Surface *loadedSurface=NULL;

        ///****************************MAP-Textures****************************///
    loadedSurface=IMG_Load("textures/map.png");
    if(loadedSurface==NULL)
    {
        printf("Cannot find map.png\n\n");
        SDL_Quit();
        exit(1);

    }
    game->gTileTexture.mTexture=SDL_CreateTextureFromSurface(game->renderer,loadedSurface);

    game->TileClip[0].x=0;
    game->TileClip[0].y=0;
    game->TileClip[0].w=32;
    game->TileClip[0].h=32;

    game->TileClip[1].x=0;
    game->TileClip[1].y=32;
    game->TileClip[1].w=32;
    game->TileClip[1].h=32;

    game->TileClip[2].x=32;
    game->TileClip[2].y=32;
    game->TileClip[2].w=32;
    game->TileClip[2].h=32;

    game->TileClip[3].x=32;
    game->TileClip[3].y=0;
    game->TileClip[3].w=32;
    game->TileClip[3].h=32;

    SDL_FreeSurface( loadedSurface );


    loadedSurface=IMG_Load("textures/Castle.png");
    if(loadedSurface==NULL)
    {
        printf("Cannot find Castle.png\n\n");
        SDL_Quit();
        exit(1);

    }
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0x0, 0xFF));
    game->gCastleTexture.mTexture=SDL_CreateTextureFromSurface(game->renderer,loadedSurface);

    game->CastleTileClip[0].x=0;
    game->CastleTileClip[0].y=0;
    game->CastleTileClip[0].w=32;
    game->CastleTileClip[0].h=32;

    game->CastleTileClip[1].x=32;
    game->CastleTileClip[1].y=0;
    game->CastleTileClip[1].w=32;
    game->CastleTileClip[1].h=32;

    game->CastleTileClip[2].x=64;
    game->CastleTileClip[2].y=0;
    game->CastleTileClip[2].w=32;
    game->CastleTileClip[2].h=32;

    game->CastleTileClip[3].x=0;
    game->CastleTileClip[3].y=32;
    game->CastleTileClip[3].w=32;
    game->CastleTileClip[3].h=32;

    game->CastleTileClip[4].x=32;
    game->CastleTileClip[4].y=32;
    game->CastleTileClip[4].w=32;
    game->CastleTileClip[4].h=32;

    game->CastleTileClip[5].x=64;
    game->CastleTileClip[5].y=32;
    game->CastleTileClip[5].w=32;
    game->CastleTileClip[5].h=32;

    game->CastleTileClip[6].x=0;
    game->CastleTileClip[6].y=64;
    game->CastleTileClip[6].w=32;
    game->CastleTileClip[6].h=32;

    game->CastleTileClip[7].x=32;
    game->CastleTileClip[7].y=64;
    game->CastleTileClip[7].w=32;
    game->CastleTileClip[7].h=32;

    game->CastleTileClip[8].x=64;
    game->CastleTileClip[8].y=64;
    game->CastleTileClip[8].w=32;
    game->CastleTileClip[8].h=32;

    SDL_FreeSurface( loadedSurface );
        ///***********************************Fighter Texture********************************///

    loadedSurface=IMG_Load("textures/player1.png");
    if(loadedSurface==NULL)
    {
        printf("Cannot find tiles textures/player1.png\n\n");
        SDL_Quit();
        exit(1);

    }

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0x0, 0xFF));

    game->mPlayerTexture=SDL_CreateTextureFromSurface(game->renderer,loadedSurface);
    if(game->mPlayerTexture==NULL)
    {
        printf( "Unable to create texture from game->playerEntity.object.mTexture! SDL Error: \n");

    }
    SDL_FreeSurface( loadedSurface );
            ///***********************************Mage Texture********************************///

    loadedSurface=IMG_Load("textures/WizardSpriteSheet.png");
    if(loadedSurface==NULL)
    {
        printf("Cannot find tiles textures/WizardSpriteSheet.png\n\n");
        SDL_Quit();
        exit(1);

    }

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0x0, 0xFF));

    game->mageTexture=SDL_CreateTextureFromSurface(game->renderer,loadedSurface);
    if(game->mageTexture==NULL)
    {
        printf( "Unable to create texture from game->mageTexture! SDL Error: \n");

    }
    SDL_FreeSurface( loadedSurface );
        ///***********************************Fireball Texture********************************///

    loadedSurface=IMG_Load("textures/fireball.png");
    if(loadedSurface==NULL)
    {
        printf("Cannot find tiles textures/fireball.png\n\n");
        SDL_Quit();
        exit(1);

    }
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0x0, 0xFF));

    game->attackTexture=SDL_CreateTextureFromSurface(game->renderer,loadedSurface);
    if(game->mPlayerTexture==NULL)
    {
        printf( "Unable to create texture from game->playerEntity.object.mTexture! SDL Error: \n");

    }
    SDL_FreeSurface( loadedSurface );

    ///*****************************************AI texture********************************///
    loadedSurface=IMG_Load("textures/Slime_Spritesheet.png");
    if(loadedSurface==NULL)
    {
        printf("Cannot find tiles textures/Slime_Spritesheet.png\n\n");
        SDL_Quit();
        exit(1);

    }
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0x0, 0xFF));
        game->mAiTexture=SDL_CreateTextureFromSurface(game->renderer,loadedSurface);
        if(game->mAiTexture==NULL)
        {
            printf( "Unable to create texture from game->AiEntity.object.mTexture! SDL Error: \n");

        }
    SDL_FreeSurface( loadedSurface );

}

void renderTimer(GameState *g){ // ritar ut en klockan som visar tid
    int textW,textH,fontSize=20,minuteLimit=60000;
    static Uint32 minutes = 0,seconds=-1;
    static int secondText,minuteText;
    char time[6];
    printf("seconds: %d\n",seconds);
    if(seconds==-1){
        seconds=g->Time.gameCurrentTime;
    }
    if(g->Time.gameCurrentTime>(seconds+1000)){
        seconds=g->Time.gameCurrentTime;
        secondText++;
    }
    if((g->Time.gameCurrentTime)>(minutes+60000)){
        minutes=g->Time.gameCurrentTime;
        minuteText++;
        secondText=0;
        printf("Increasing minutes: %d\n",minutes);
    }
    snprintf(time, 6, "%d:%d",minuteText,secondText);
    g->Text.textSurface = TTF_RenderText_Solid(g->Text.font,time,g->Text.color);
    g->Text.textTexture = SDL_CreateTextureFromSurface(g->renderer,g->Text.textSurface);
    SDL_QueryTexture(g->Text.textTexture,NULL,NULL,&textW,&textH);
    SDL_Rect textRect={SCREEN_WIDTH/2-textW/2,0,textW,textH};
    SDL_RenderCopy(g->renderer,g->Text.textTexture,NULL,&textRect);


}



void doRender(SDL_Renderer *renderer, GameState *game,SDL_Rect mCam) //  Ritar ut saker och rendrar dom på skärmen
{
    int textW,textH,fontSize=15,i;
    SDL_Color color = {255,255,255};
    TTF_Font *font = TTF_OpenFont("textures/LOUDNOISEBLACK.ttf", fontSize);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Surface *chatSurface;
    SDL_Texture *chatTexture,*history1,*history2,*history3;
  //set the drawing color to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    //Clear the screen (to blue)
    SDL_RenderClear(renderer);
    for(int i=0;i<COLUMNS;i++) // rendrar kartan
    {
        for(int j=0;j<ROWS;j++)
        {
            if(playfield[i][j]==0)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[0],&camera2);
            }
             if(playfield[i][j]==1)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[1],&camera2);
            }
            if(playfield[i][j]==2)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[2],&camera2);
            }
            if(playfield[i][j]==3)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[3],&camera2);
            }
            if(playfield[i][j]==4)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[2],&camera2);
                SDL_RenderCopy(renderer,game->gCastleTexture.mTexture,&game->CastleTileClip[0],&camera2);
            }
            if(playfield[i][j]==5)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[2],&camera2);
                SDL_RenderCopy(renderer,game->gCastleTexture.mTexture,&game->CastleTileClip[1],&camera2);
            }
            if(playfield[i][j]==6)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[2],&camera2);
                SDL_RenderCopy(renderer,game->gCastleTexture.mTexture,&game->CastleTileClip[2],&camera2);
            }
            if(playfield[i][j]==7)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[2],&camera2);
                SDL_RenderCopy(renderer,game->gCastleTexture.mTexture,&game->CastleTileClip[3],&camera2);
            }
            if(playfield[i][j]==8)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[2],&camera2);
                SDL_RenderCopy(renderer,game->gCastleTexture.mTexture,&game->CastleTileClip[4],&camera2);
            }
            if(playfield[i][j]==9)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[2],&camera2);
                SDL_RenderCopy(renderer,game->gCastleTexture.mTexture,&game->CastleTileClip[5],&camera2);
            }
            if(playfield[i][j]==10)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[2],&camera2);
                SDL_RenderCopy(renderer,game->gCastleTexture.mTexture,&game->CastleTileClip[6],&camera2);
            }
            if(playfield[i][j]==11)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[2],&camera2);
                SDL_RenderCopy(renderer,game->gCastleTexture.mTexture,&game->CastleTileClip[7],&camera2);
            }
            if(playfield[i][j]==12)
            {
                SDL_Rect camera2={j*TILESIZE-mCam.x,i*TILESIZE-mCam.y,TILESIZE,TILESIZE};
                SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&game->TileClip[2],&camera2);
                SDL_RenderCopy(renderer,game->gCastleTexture.mTexture,&game->CastleTileClip[8],&camera2);
            }

        }

    }

//    SDL_Rect camera={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
//    SDL_RenderCopy(renderer,game->gTileTexture.mTexture,&mCam,&camera);

    SDL_SetRenderDrawColor(renderer, 255,255, 255, 255);
        SDL_Rect tmp;
    //set the drawing color to white
   // SDL_Rect rect = {game->playerEntity.mPosX-mCam.x,game->playerEntity.mPosY-mCam.y, 32, 32 };

//    SDL_Rect AIrect = {game->AiEntity.mPosX-mCam.x,game->AiEntity.mPosY-mCam.y, 32, 32 };
//    SDL_RenderCopy(renderer,game->AiEntity.object.mTexture,&game->AiEntity.SpriteFacing,&AIrect);
   // SDL_RenderFillRect(renderer, &rect);
   for(int i=0;i<300;i++) // rendrar ai
   {
        if(game->aiEntityToken[i]==1){

            whatSprite(&game->AiEntity[i]);
            SDL_RenderCopy(renderer,game->mAiTexture,&game->AiEntity[i].spriteFacing,getRenderPosition(game->AiEntity[i].object.rect,mCam,tmp));
            updatehealthbar(&game->AiEntity[i]);
            if(SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF )){
                printf("Could not set render draw color for healthbar max. SDL_ERROR: %s\n", SDL_GetError());
            }

            renderShape(renderer, game->AiEntity[i].hpData.healthBarMax, mCam );
            if(SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF )){
                printf("Could not set render draw color for healthbar current. SDL_ERROR: %s\n", SDL_GetError());
            }
            renderShape(renderer, game->AiEntity[i].hpData.healthBarCurrent, mCam );

        }else if(game->AiEntity[i].deathAnimation == 1) {

            deathAnimation(&game->AiEntity[i]); // när ain dör gör den en kort death animation
            SDL_RenderCopy(renderer,game->mAiTexture,&game->AiEntity[i].spriteFacing,getRenderPosition(game->AiEntity[i].object.rect,mCam,tmp));
        }
   }


            updatehealthbar(&game->castle); //Nedanstående ritar ut healthbars
            if(SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF )){
                printf("Could not set render draw color for healthbar max. SDL_ERROR: %s\n", SDL_GetError());
            }

            renderShape(renderer, game->castle.hpData.healthBarMax, mCam );

            if(SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF )){
                printf("Could not set render draw color for healthbar current. SDL_ERROR: %s\n", SDL_GetError());
            }
            renderShape(renderer, game->castle.hpData.healthBarCurrent, mCam );



       for(int i=0;i<game->maxPlayers;i++){ // ritar spelarna
        if(game->playerEntityToken[i]==1){
            whatSprite(&game->playerEntity[i]);
            if(game->playerEntity[i].characterType == 0){
                SDL_RenderCopy(renderer,game->mPlayerTexture,&game->playerEntity[i].spriteFacing,getRenderPosition(game->playerEntity[i].object.rect,mCam,tmp));
            }else if(game->playerEntity[i].characterType == 1){
                SDL_RenderCopy(renderer,game->mageTexture,&game->playerEntity[i].spriteFacing,getRenderPosition(game->playerEntity[i].object.rect,mCam,tmp));
            }
            SDL_RenderCopy(renderer,game->attackTexture,&game->playerEntity[i].attack.spriteFacing,getRenderPosition(game->playerEntity[i].attack.rect,mCam,tmp));
        }
    }
        renderTimer(game);
    for(i=0;i<4;i++){ // rendrar chathistoriken
            chatSurface = TTF_RenderText_Solid(font,game->Text.chatHistory[i],color);
            chatTexture = SDL_CreateTextureFromSurface(game->renderer,chatSurface);
            SDL_QueryTexture(chatTexture,NULL,NULL,&textW,&textH);
            SDL_Rect chatRect={0,SCREEN_HEIGHT-fontSize*(i+2),textW,textH};
            SDL_RenderCopy(renderer,chatTexture,NULL,&chatRect);
        }
    if(game->Text.enterPressedFlag==1){ // rendrar meddelandet man skriver i chatten
        printf("Printing chat\n");
        chatSurface = TTF_RenderText_Solid(font,game->Text.chatMessage,color);
        chatTexture = SDL_CreateTextureFromSurface(game->renderer,chatSurface);
        SDL_QueryTexture(chatTexture,NULL,NULL,&textW,&textH);
        SDL_Rect chatRect={0,SCREEN_HEIGHT-fontSize,textW,textH};
        SDL_RenderDrawRect(renderer,&chatRect);

        SDL_RenderCopy(renderer,chatTexture,NULL,&chatRect);
    }

    SDL_RenderPresent(renderer);
        //SDL_RenderPresent(renderer);
}

SDL_Rect *getRenderPosition(SDL_Rect entity,SDL_Rect mCam,SDL_Rect temp)
{
    temp = entity;
    temp.x-=mCam.x;
    temp.y-=mCam.y;

    return &temp;
}

void renderShape(SDL_Renderer *renderer, SDL_Rect rect, SDL_Rect mCam)
{
    SDL_Rect tmp;
    tmp = rect;
    tmp.x-=mCam.x;
    tmp.y-=mCam.y;
    SDL_RenderFillRect(renderer,&tmp);
}

void updatehealthbar(Entity *aiEntity) // updaterar healthbar
{
    int sizeOfGreenbar;
    float currentHpPercent;
    currentHpPercent = (aiEntity->hpData.currentHp)/(aiEntity->hpData.maxHp); // Hur många procent av max hpt man har för tillfället
    sizeOfGreenbar = currentHpPercent*aiEntity->hpData.sizeOfHealthbar; // hur stor yta av hela healthbaren procenten motsvarar
    aiEntity->hpData.healthBarCurrent.w = sizeOfGreenbar; //sätter storleken av den gröna baren till samma storlek den bör vara

    aiEntity->hpData.healthBarMax.x = aiEntity->object.rect.x;
    aiEntity->hpData.healthBarMax.y = aiEntity->object.rect.y - 16;
    aiEntity->hpData.healthBarCurrent.x = aiEntity->object.rect.x;
    aiEntity->hpData.healthBarCurrent.y = aiEntity->object.rect.y - 16;
}

void deathAnimation(Entity *AI) // hanterar deathanimation
{
    int ms = SDL_GetTicks();
    int sprite = ms /400 % 4;

    AI->spriteFacing.x = sprite*32;
    AI->spriteFacing.y = 128;
    AI->spriteFacing.w = 32;
    AI->spriteFacing.h = 32;

    if(sprite == 3)
        AI->deathAnimation = 0;
}

void whatSprite(Entity *AI)
{
    int ms = SDL_GetTicks();
    int sprite = ms /150 % 3;


    if((SDL_GetTicks() - AI->attackTimer) <100){
        sprite = ms /200 % 2 + 2;
            AI->spriteFacing.x = sprite*32;
        }
    else if(AI->facing == 1)
       {
            AI->spriteFacing.x = sprite*32;
            AI->spriteFacing.y = 96;
            AI->spriteFacing.w = 32;
            AI->spriteFacing.h = 32;
        }
    else if(AI->facing == 3)
       {
           AI->spriteFacing.x = sprite*32;
           AI->spriteFacing.y = 64;
           AI->spriteFacing.w = 32;
           AI->spriteFacing.h = 32;
       }
    else if(AI->facing == 2)
       {
           AI->spriteFacing.x = sprite*32;
           AI->spriteFacing.y = 32;
           AI->spriteFacing.w = 32;
           AI->spriteFacing.h = 32;

       }
    else if(AI->facing == 0)
       {
           AI->spriteFacing.x = sprite*32;
           AI->spriteFacing.y = 0;
           AI->spriteFacing.w = 32;
           AI->spriteFacing.h = 32;
       }

}

