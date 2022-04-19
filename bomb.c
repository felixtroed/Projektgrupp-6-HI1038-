#include "bomb.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define BOMB_WIDTH 32
#define BOMB_HEIGHT 32

PUBLIC Bomb createBomb(int x, int y, Game game) {
    Bomb bomb = malloc(sizeof(struct Bomb));

    bomb->texture = NULL;
    bomb->surface = IMG_Load("resources/bomb-temp.png");
    if(bomb->surface == NULL ) {
            printf( "Unable to load bomb image. SDL_image error: %s\n", IMG_GetError());
    }
    else {
        bomb->texture = SDL_CreateTextureFromSurface(game->renderer, bomb->surface);
        if(bomb->texture == NULL ) {
            printf( "Unable to create bomb texture. SDL error: %s\n", SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(bomb->surface);
    }

    bomb->pos.x = x;
    bomb->pos.y = y;
    bomb->pos.w = BOMB_WIDTH;
    bomb->pos.h = BOMB_HEIGHT;
    bomb->currentFrame = 0;

    return bomb;
}

PRIVATE void initClips(Bomb bomb) {

}