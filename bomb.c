#include "bomb.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define BOMB_WIDTH 50
#define BOMB_HEIGHT 50

PRIVATE Uint32 redBomb(Uint32 interval, void* switchToRedBomb);
PRIVATE Uint32 bombExploded(Uint32 interval, void *bomb);
PRIVATE Bomb createBomb(int x, int y, Game game);
PUBLIC void placeBomb(Game game);
PUBLIC void renderBombs(Game game);

PUBLIC void placeBomb(Game game) {
    game->bombs[0] = createBomb(game->p1->pos.x + 12, game->p1->pos.y + 12, game);
    game->bombs[0]->switchRedBomb = false;                                                                  // Röd bomb avaktiverad från början
    game->bombs[0]->redBombTime = SDL_AddTimer(2000, redBomb, game->bombs[0]);                              // Timer tills röd bomb ska visas
    game->bombs[0]->explosionTime = SDL_AddTimer(3000, bombExploded, game->bombs[0]);
}

PRIVATE Uint32 redBomb(Uint32 interval, void *switchToRedBomb) {
    Bomb b = (Bomb) switchToRedBomb;
    b->switchRedBomb = true;
    return 0;
}

PRIVATE Uint32 bombExploded(Uint32 interval, void *bomb) {
    Bomb b = (Bomb) bomb;
    printf("Callback function entered.\n");
    b->exploded = true;
    return 0;
}

PUBLIC void renderBombs(Game game) {
    if (game->bombs[0] != NULL) {
        if (game->bombs[0]->switchRedBomb) {
            SDL_RenderCopy(game->renderer, game->bombs[0]->textureBombRed, NULL, &game->bombs[0]->pos);     // Renderar röd bomb
        }
        else {
            SDL_RenderCopy(game->renderer, game->bombs[0]->textureBomb, NULL, &game->bombs[0]->pos);        // Renderar svart bomb
        }
        if (game->bombs[0]->exploded) {
            printf("Timer done.\n");
            game->bombs[0] = NULL;
        }
    }
}

PRIVATE Bomb createBomb(int x, int y, Game game) {
    Bomb bomb = malloc(sizeof(struct Bomb));

    bomb->surface = IMG_Load("resources/bomb.png");                                     // Laddar in svart bomb
    if(bomb->surface == NULL ) {
        printf( "Unable to load bomb image. SDL_image error: %s\n", IMG_GetError());
    }
    else {
        bomb->textureBomb = SDL_CreateTextureFromSurface(game->renderer, bomb->surface);
        if (bomb->textureBomb == NULL) {
            printf("Unable to create bomb texture. SDL error: %s\n", SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(bomb->surface);
    }

    bomb->surface = IMG_Load("resources/bomb-red.png");                                 // Laddar in röd bomb
    if (bomb->surface == NULL) {
        printf("Unable to load bomb-red image. SDL_image error: %s\n", IMG_GetError());
    }
    else {
        bomb->textureBombRed = SDL_CreateTextureFromSurface(game->renderer, bomb->surface);
        if (bomb->textureBombRed == NULL) {
            printf("Unable to create bomb-red texture. SDL error: %s\n", SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(bomb->surface);
    }

    bomb->pos.x = ((game->p1->pos.x + 32) / 64) * 64 + 7;       // Formel för att placera bomben i närmaste ruta (utgår från karaktärens mittpunkt)
    bomb->pos.y = ((game->p1->pos.y + 32) / 64) * 64 + 7;       // +7 för bomben är 50x50 pixlar (64-50=7) Måste ändras ifall karaktärens eller bombens storlek ändras!!!
    bomb->pos.w = BOMB_WIDTH;
    bomb->pos.h = BOMB_HEIGHT;
    bomb->currentFrame = 0;
    bomb->exploded = false;
    // bomb->explosionTime = SDL_AddTimer(3000, bombExploded, game->bombs[0]);
    // bomb->hasCollision = false; // Maybe only use "exploded" boolean
    bomb->explosionRange = 2;

    return bomb;
}



PUBLIC void initBombs(Bomb bombs[]) {
    for (int i = 0; i < BOMBS; i++) {
        bombs[i] = NULL;
    }
}

PRIVATE void initClips(Bomb bomb) {

}
