#include "bomb.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define BOMB_WIDTH 40
#define BOMB_HEIGHT 40

PRIVATE Uint32 bombExploded(Uint32 interval, void *bomb);
PRIVATE Bomb createBomb(int x, int y, Game game);
PUBLIC void placeBomb(Game game);

PUBLIC void placeBomb(Game game) {
    game->bombs[0] = createBomb(64+12, 128+12, game);
    game->bombs[0]->explosionTime = SDL_AddTimer(3000, bombExploded, game->bombs[0]);
}

PRIVATE Bomb createBomb(int x, int y, Game game) {
    Bomb bomb = malloc(sizeof(struct Bomb));

    bomb->texture = NULL;
    bomb->surface = IMG_Load("resources/bomb/bomb (png)/bomb.png");
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
    bomb->exploded = false;
    // bomb->explosionTime = SDL_AddTimer(3000, bombExploded, game->bombs[0]);
    // bomb->hasCollision = false; // Maybe only use "exploded" boolean
    bomb->explosionRange = 2;

    return bomb;
}

PRIVATE Uint32 bombExploded(Uint32 interval, void *bomb) {
    Bomb b = (Bomb) bomb;
    printf("Callback function entered.\n");
    b->exploded = true;
    return 0;
}

PUBLIC void initBombs(Bomb bombs[]) {
    for (int i = 0; i < BOMBS; i++) {
        bombs[i] = NULL;
    }
}

PRIVATE void initClips(Bomb bomb) {

}