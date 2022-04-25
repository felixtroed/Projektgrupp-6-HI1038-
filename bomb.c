#include "bomb.h"
#include "player.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define BOMB_WIDTH 50
#define BOMB_HEIGHT 50

typedef struct BombTimerCallbackArgs {
    Bomb bomb;
    uint8_t *bombsAvailable;
} BombTimerCallbackArgs;

PRIVATE Uint32 redBomb(Uint32 interval, void *switchToRedBomb);
PRIVATE Uint32 bombExploded(Uint32 interval, void *args);
PRIVATE Uint32 explosionDone(Uint32 interval, void *deleteBomb);
PRIVATE Bomb createBomb(int playerPosX, int playerPosY, SDL_Renderer *renderer);
PRIVATE uint8_t getBombIdx(Bomb bombs[]);
PUBLIC void bombPlacement(Player p, Bomb bombs[], SDL_Renderer *renderer);
PUBLIC void renderBombsAndExplosions(Game game);

PUBLIC void bombPlacement(Player p, Bomb bombs[], SDL_Renderer *renderer) {
    if (p->bombsAvailable) {
        uint8_t bombIdx = getBombIdx(bombs);             // Get first free index to store bomb
        (p->bombsAvailable)--;

        BombTimerCallbackArgs *callbackArgs = malloc(sizeof(BombTimerCallbackArgs));
        callbackArgs->bomb = bombs[bombIdx] = createBomb(p->pos.x, p->pos.y, renderer);
        callbackArgs->bombsAvailable = &p->bombsAvailable;

        bombs[bombIdx]->redBombTime = SDL_AddTimer(2000, redBomb, callbackArgs);                      // Timer tills r�d bomb ska visas
        bombs[bombIdx]->bombTime = SDL_AddTimer(3000, bombExploded, callbackArgs);                    // Timer tills explosion
        bombs[bombIdx]->deleteBombTime = SDL_AddTimer(4000, explosionDone, callbackArgs);
    }
}

PRIVATE uint8_t getBombIdx(Bomb bombs[]) {
    for (uint8_t i = BOMBS-1; i > 0; i--) {
        if (bombs[i] == NULL) {
            return i;
        }
    }
    return 0;
}

PRIVATE Uint32 redBomb(Uint32 interval, void *args) {
    BombTimerCallbackArgs* bargs = (BombTimerCallbackArgs*) args;
    bargs->bomb->switchRedBomb = true;

    return 0;
}

PRIVATE Uint32 bombExploded(Uint32 interval, void *args) {
    BombTimerCallbackArgs* bargs = (BombTimerCallbackArgs*) args;
    bargs->bomb->startExplosion = true;

    return 0;
}

PRIVATE Uint32 explosionDone(Uint32 interval, void *args) {
    BombTimerCallbackArgs *bargs = (BombTimerCallbackArgs*) args;
    bargs->bomb->endExplosion = true;
    (*(bargs->bombsAvailable))++;

    printf("Callback function entered.\n");
    free(bargs);
    return 0;
}

PUBLIC void renderBombsAndExplosions(Game game) {
    for (uint8_t i = 0; i < BOMBS; i++) {
        if (game->bombs[i] != NULL) {
            if (game->bombs[i]->switchRedBomb) {
                SDL_RenderCopy(game->renderer, game->bombs[i]->textureBombRed, NULL, &game->bombs[i]->bombPos);     // Renderar r�d bomb
            }
            else {
                SDL_RenderCopy(game->renderer, game->bombs[i]->textureBomb, NULL, &game->bombs[i]->bombPos);        // Renderar svart bomb
            }
            if (game->bombs[i]->startExplosion) {

                int range = game->bombs[i]->explosionRange;
                game->bombs[i]->explosionPos.y = game->bombs[i]->bombPos.y - 7;
                SDL_RenderCopy(game->renderer, game->bombs[i]->textureExplosionStart, NULL, &game->bombs[i]->explosionPos);         // Renderar explosion-start

                // skapa en if-sats. Om nästa ruta har kolission -> sluta rendera åt det hållet

                // Renderar explosion åt höger
                for (int j = 0; j < range - 1; j++) {
                    game->bombs[i]->explosionPos.x += 64;
                    SDL_RenderCopy(game->renderer, game->bombs[i]->textureExplosionMiddle, NULL, &game->bombs[i]->explosionPos);
                }
                game->bombs[i]->explosionPos.x += 64;
                SDL_RenderCopy(game->renderer, game->bombs[i]->textureExplosionEnd, NULL, &game->bombs[i]->explosionPos);
                game->bombs[i]->explosionPos.x = game->bombs[i]->bombPos.x - 7;                                                     // Återställer explosionPos.x

                // Renderar explosion åt vänster
                for (int j = 0; j < range - 1; j++) {
                    game->bombs[i]->explosionPos.x -= 64;
                    SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionMiddle, NULL, &game->bombs[i]->explosionPos, 180, NULL, SDL_FLIP_NONE);
                }
                game->bombs[i]->explosionPos.x -= 64;
                SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionEnd, NULL, &game->bombs[i]->explosionPos, 180, NULL, SDL_FLIP_NONE);
                game->bombs[i]->explosionPos.x = game->bombs[i]->bombPos.x - 7;                                                     // Återställer explosionPos.x

                // Renderar explosion uppåt
                for (int j = 0; j < range - 1; j++) {
                    game->bombs[i]->explosionPos.y -= 64;
                    SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionMiddle, NULL, &game->bombs[i]->explosionPos, 270, NULL, SDL_FLIP_NONE);
                }
                game->bombs[i]->explosionPos.y -= 64;
                SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionEnd, NULL, &game->bombs[i]->explosionPos, 270, NULL, SDL_FLIP_NONE);
                game->bombs[i]->explosionPos.y = game->bombs[i]->bombPos.y - 7;                                                     // Återställer explosionPos.y

                // Renderar explosion nedåt
                for (int j = 0; j < range - 1; j++) {
                    game->bombs[i]->explosionPos.y += 64;
                    SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionMiddle, NULL, &game->bombs[i]->explosionPos, 90, NULL, SDL_FLIP_NONE);
                }
                game->bombs[i]->explosionPos.y += 64;
                SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionEnd, NULL, &game->bombs[i]->explosionPos, 90, NULL, SDL_FLIP_NONE);
                game->bombs[i]->explosionPos.y = game->bombs[i]->bombPos.y - 7;                                                     // Återställer explosionPos.y

            } 
            if (game->bombs[i]->endExplosion) {
                game->bombs[i] = NULL;                                                                          // Raderar bomben
                printf("Explosion timer done. Bomb deleted.\n");
            } 

          

        }
    }
}

PRIVATE Bomb createBomb(int playerPosX, int playerPosY, SDL_Renderer *renderer) {
    Bomb bomb = malloc(sizeof(struct Bomb));

    bomb->surface = IMG_Load("resources/bomb.png");                                     // Laddar in svart bomb
    if (bomb->surface == NULL) {
        printf("Unable to load bomb image. SDL_image error: %s\n", IMG_GetError());
    }
    else {
        bomb->textureBomb = SDL_CreateTextureFromSurface(renderer, bomb->surface);
        if (bomb->textureBomb == NULL) {
            printf("Unable to create bomb texture. SDL error: %s\n", SDL_GetError());
        }
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
        SDL_FreeSurface(bomb->surface);
    }

    bomb->surface = IMG_Load("resources/explosion-start.png");                                     // Laddar in start explosionen
    if (bomb->surface == NULL) {
        printf("Unable to load explosion-start. SDL_image error: %s\n", IMG_GetError());
    }
    else {
        bomb->textureExplosionStart = SDL_CreateTextureFromSurface(renderer, bomb->surface);
        if (bomb->textureExplosionStart == NULL) {
            printf("Unable to create explosion-start texture. SDL error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(bomb->surface);
    }

    bomb->surface = IMG_Load("resources/explosion-middle.png");                                     // Laddar in mitten explosionen
    if (bomb->surface == NULL) {
        printf("Unable to load explosion-middle SDL_image error: %s\n", IMG_GetError());
    }
    else {
        bomb->textureExplosionMiddle = SDL_CreateTextureFromSurface(renderer, bomb->surface);
        if (bomb->textureExplosionMiddle == NULL) {
            printf("Unable to create explosion-middle texture. SDL error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(bomb->surface);
    }

    bomb->surface = IMG_Load("resources/explosion-end.png");                                     // Laddar in slut explosionen
    if (bomb->surface == NULL) {
        printf("Unable to load explosion-end. SDL_image error: %s\n", IMG_GetError());
    }
    else {
        bomb->textureExplosionEnd = SDL_CreateTextureFromSurface(renderer, bomb->surface);
        if (bomb->textureExplosionEnd == NULL) {
            printf("Unable to create explosion-end texture. SDL error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(bomb->surface);
    }

    bomb->bombPos.x = ((playerPosX + 32) / 64) * 64 + 7;       // Formel f�r att placera bomben i n�rmaste ruta (utg�r fr�n karakt�rens mittpunkt)
    bomb->bombPos.y = ((playerPosY + 32) / 64) * 64 + 7;       // +7 f�r bomben �r 50x50 pixlar (64-50=7) M�ste �ndras ifall karakt�rens eller bombens storlek �ndras!!!
    bomb->bombPos.w = BOMB_WIDTH;
    bomb->bombPos.h = BOMB_HEIGHT;
    bomb->explosionPos.x = bomb->bombPos.x - 7;
    bomb->explosionPos.y = bomb->bombPos.y - 7;
    bomb->explosionPos.w = 64;
    bomb->explosionPos.h = 64;
    bomb->currentFrame = 0;
    bomb->switchRedBomb = false;                                                  // R�d bomb avaktiverad fr�n b�rjan
    bomb->startExplosion = false;
    bomb->endExplosion = false;
    // bomb->hasCollision = false; // Maybe only use "exploded" boolean
    bomb->explosionRange = 2;

    return bomb;
}

PUBLIC void initBombs(Bomb bombs[]) {
    for (int i = 0; i < BOMBS; i++) {
        bombs[i] = NULL;
    }
}