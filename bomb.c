#include "bomb.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define BOMB_WIDTH 50
#define BOMB_HEIGHT 50

typedef struct BombTimerCallbackArgs {
    Bomb bomb;
    uint8_t *bombsAvailable;
} BombTimerCallbackArgs;

PRIVATE Uint32 redBomb(Uint32 interval, void* switchToRedBomb);
PRIVATE Uint32 bombExploded(Uint32 interval, void *args);
PRIVATE Bomb createBomb(int playerPosX, int playerPosY, SDL_Renderer *renderer);
PUBLIC void bombPlacement(Player p, Bomb bombs[], SDL_Renderer *renderer);
PUBLIC void renderBombs(Game game);

PUBLIC void bombPlacement(Player p, Bomb bombs[], SDL_Renderer *renderer) {
    if (p->bombsAvailable) {
        (p->bombsAvailable)--;

        BombTimerCallbackArgs *callbackArgs = malloc(sizeof(BombTimerCallbackArgs));
        callbackArgs->bomb = bombs[0] = createBomb(p->pos.x, p->pos.y, renderer);
        callbackArgs->bombsAvailable = &p->bombsAvailable;

        bombs[0]->switchRedBomb = false;                                                  // R�d bomb avaktiverad fr�n b�rjan
        bombs[0]->redBombTime = SDL_AddTimer(2000, redBomb, bombs[0]);                    // Timer tills r�d bomb ska visas
        bombs[0]->explosionTime = SDL_AddTimer(3000, bombExploded, callbackArgs);
    }
}

PRIVATE Uint32 redBomb(Uint32 interval, void *switchToRedBomb) {
    Bomb b = (Bomb) switchToRedBomb;
    b->switchRedBomb = true;
    return 0;
}

PRIVATE Uint32 bombExploded(Uint32 interval, void *args) {
    BombTimerCallbackArgs *bargs = (BombTimerCallbackArgs*) args;
    bargs->bomb->exploded = true;
    (*(bargs->bombsAvailable))++;

    printf("Callback function entered.\n");
    free(bargs);
    return 0;
}

PUBLIC void renderBombs(Game game) {
    for (uint8_t i = 0; i < BOMBS; i++) {
        if (game->bombs[i] != NULL) {
            if (game->bombs[i]->switchRedBomb) {
                SDL_RenderCopy(game->renderer, game->bombs[i]->textureBombRed, NULL, &game->bombs[i]->pos);     // Renderar r�d bomb
            }
            else {
                SDL_RenderCopy(game->renderer, game->bombs[i]->textureBomb, NULL, &game->bombs[i]->pos);        // Renderar svart bomb
            }
            if (game->bombs[i]->exploded) {
                game->bombs[i] = NULL;
            }
        }
    }
}

PRIVATE Bomb createBomb(int playerPosX, int playerPosY, SDL_Renderer *renderer) {
    Bomb bomb = malloc(sizeof(struct Bomb));

    bomb->surface = IMG_Load("resources/bomb.png");                                     // Laddar in svart bomb
    if(bomb->surface == NULL ) {
        printf( "Unable to load bomb image. SDL_image error: %s\n", IMG_GetError());
    }
    else {
        bomb->textureBomb = SDL_CreateTextureFromSurface(renderer, bomb->surface);
        if (bomb->textureBomb == NULL) {
            printf("Unable to create bomb texture. SDL error: %s\n", SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(bomb->surface);
    }

    bomb->surface = IMG_Load("resources/bomb-red.png");                                 // Laddar in r�d bomb
    if (bomb->surface == NULL) {
        printf("Unable to load bomb-red image. SDL_image error: %s\n", IMG_GetError());
    }
    else {
        bomb->textureBombRed = SDL_CreateTextureFromSurface(renderer, bomb->surface);
        if (bomb->textureBombRed == NULL) {
            printf("Unable to create bomb-red texture. SDL error: %s\n", SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(bomb->surface);
    }

    bomb->pos.x = ((playerPosX + 32) / 64) * 64 + 7;       // Formel f�r att placera bomben i n�rmaste ruta (utg�r fr�n karakt�rens mittpunkt)
    bomb->pos.y = ((playerPosY + 32) / 64) * 64 + 7;       // +7 f�r bomben �r 50x50 pixlar (64-50=7) M�ste �ndras ifall karakt�rens eller bombens storlek �ndras!!!
    bomb->pos.w = BOMB_WIDTH;
    bomb->pos.h = BOMB_HEIGHT;
    bomb->currentFrame = 0;
    bomb->exploded = false;
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
