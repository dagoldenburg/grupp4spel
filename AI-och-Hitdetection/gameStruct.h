#ifndef GAMESTRUCT_H_INCLUDED
#define GAMESTRUCT_H_INCLUDED

typedef struct
{
    int mWidth;
    int mHeight;
    SDL_Texture* mTexture;

}LTexture;

typedef struct {
    int sizeOfHealthbar;
    float maxHp;
    float currentHp;
    SDL_Rect healthBarCurrent;
    SDL_Rect healthBarMax;
} HitPointData;



typedef struct
{   SDL_Rect rect;
    int mVelX;
    int mVelY;
    int strength;

    SDL_Rect attack;
    HitPointData hpData;
}Entity;

typedef struct
{
    int mtype;
    SDL_Rect mBox;
}Tile;

typedef struct
{
    Entity Entity;
    Tile tileSet[TOTAL_TILES];
    LTexture gPlayerTexture;
    LTexture gTileTexture;
    SDL_Rect TileClip[4];
    SDL_Rect Source;
    int XPOStmp;
    int YPOStmp;
    SDL_Renderer *renderer;


}GameState;


#endif // GAMESTRUCT_H_INCLUDED
