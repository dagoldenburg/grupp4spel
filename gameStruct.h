#ifndef GAMESTRUCT_H_INCLUDED
#define GAMESTRUCT_H_INCLUDED

typedef struct
{
    int mWidth;
    int mHeight;
    SDL_Texture* mTexture;

}LTexture;
typedef struct
{
    int mPosX;
    int mPosY;

    int mVelX;
    int mVelY;
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
    LTexture gDotTexture;
    LTexture gTileTexture;
    SDL_Rect TileClip[4];
    SDL_Renderer *renderer;

}GameState;


#endif // GAMESTRUCT_H_INCLUDED
