#include "bomb.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define BOMB_WIDTH 50
#define BOMB_HEIGHT 50

typedef struct BombTimerCallbackArgs {
    Bomb bomb;
    uint8_t *bombsAvailable;
} BombTimerCallbackArgs;

PRIVATE Uint32 redBomb(Uint32 interval, void *switchToRedBomb);
PRIVATE Uint32 explodeBomb(Uint32 interval, void *args);
PRIVATE Uint32 explosionDone(Uint32 interval, void *deleteBomb);
PRIVATE Bomb createBomb(int playerPosX, int playerPosY, SDL_Renderer *renderer);
PRIVATE uint8_t getBombIdx(Bomb bombs[]);
PRIVATE void handleExplosions(Bomb bomb);
PUBLIC void bombPlacement(Player p, Bomb bombs[], SDL_Renderer *renderer);
PUBLIC void renderBombsAndExplosions(Game game);

PUBLIC void bombPlacement(Player p, Bomb bombs[], SDL_Renderer *renderer) {
    if (p->bombsAvailable) {
        uint8_t bombIdx = getBombIdx(bombs);             // Get first free index to store bomb
        (p->bombsAvailable)--;

        BombTimerCallbackArgs *callbackArgs = malloc(sizeof(BombTimerCallbackArgs));
        callbackArgs->bomb = bombs[bombIdx] = createBomb(p->pos.x, p->pos.y, renderer);
        callbackArgs->bombsAvailable = &p->bombsAvailable;

        bombs[bombIdx]->redBombTime = SDL_AddTimer(2000, redBomb, callbackArgs);                     // Timer tills r�d bomb ska visas
        bombs[bombIdx]->bombTime = SDL_AddTimer(3000, explodeBomb, callbackArgs);                    // Timer tills explosion
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

PRIVATE Uint32 explodeBomb(Uint32 interval, void *args) {
    BombTimerCallbackArgs* bargs = (BombTimerCallbackArgs*) args;
    bargs->bomb->startExplosion = true;

    handleExplosions(bargs->bomb);    // Modifies explosion hitbox based on collision with walls and boxes as well as deletes boxes

    return 0;
}

PRIVATE Uint32 explosionDone(Uint32 interval, void *args) {
    BombTimerCallbackArgs *bargs = (BombTimerCallbackArgs*) args;
    bargs->bomb->endExplosion = true;
    (*(bargs->bombsAvailable))++;

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
                bool stoneWall = false;
                bool hitWall = false;

                SDL_RenderCopy(game->renderer, game->bombs[i]->textureExplosionStart, NULL, &game->bombs[i]->explosionPos);         // Renderar explosion-start

                // Renderar explosion åt höger
                for (int j = 0; j < range - 1; j++) {
                    game->bombs[i]->explosionPos.x += 64;

                    for (int row = 0; row < ROW_SIZE; row++) {
                        for (int column = 0; column < COLUMN_SIZE; column++) {
                            if (activeBox[row][column] > 0) {

                                int boxLeft = column * 64 + 64;
                                int boxRight = boxLeft + 64;
                                int boxUp = row * 64 + 64;
                                int boxDown = boxUp + 64;

                                if ((game->bombs[i]->explosionPos.x + 32) > boxLeft && (game->bombs[i]->explosionPos.x + 32) < boxRight && (game->bombs[i]->explosionPos.y + 32) > boxUp
                                    && (game->bombs[i]->explosionPos.y) + 32 < boxDown) {

                                    if (activeBox[row][column] == 3) {
                                        stoneWall = true;
                                    }

                                    row = ROW_SIZE;
                                    column = COLUMN_SIZE;
                                    j = range;
                                    hitWall = true;
                                    game->bombs[i]->explosionPos.x -= 64;
                                }
                            }
                        }
                    }
                    if (!hitWall && game->bombs[i]->explosionPos.x < WINDOW_WIDTH - 92) {
                        SDL_RenderCopy(game->renderer, game->bombs[i]->textureExplosionMiddle, NULL, &game->bombs[i]->explosionPos);
                    }
                }
                game->bombs[i]->explosionPos.x += 64;
                if (!stoneWall && game->bombs[i]->explosionPos.x < WINDOW_WIDTH - 92) {
                    SDL_RenderCopy(game->renderer, game->bombs[i]->textureExplosionEnd, NULL, &game->bombs[i]->explosionPos);
                }
                game->bombs[i]->explosionPos.x = game->bombs[i]->bombPos.x - 7;                                                     // Återställer startvärden
                stoneWall = false;
                hitWall = false;

                // Renderar explosion åt vänster
                for (int j = 0; j < range - 1; j++) {
                    game->bombs[i]->explosionPos.x -= 64;

                    for (int row = 0; row < ROW_SIZE; row++) {
                        for (int column = 0; column < COLUMN_SIZE; column++) {
                            if (activeBox[row][column] > 0) {

                                int boxLeft = column * 64 + 64;
                                int boxRight = boxLeft + 64;
                                int boxUp = row * 64 + 64;
                                int boxDown = boxUp + 64;

                                if ((game->bombs[i]->explosionPos.x + 32) > boxLeft && (game->bombs[i]->explosionPos.x + 32) < boxRight && (game->bombs[i]->explosionPos.y + 32) > boxUp
                                    && (game->bombs[i]->explosionPos.y) + 32 < boxDown) {

                                    if (activeBox[row][column] == 3) {
                                        stoneWall = true;
                                    }

                                    row = ROW_SIZE;
                                    column = COLUMN_SIZE;
                                    j = range;
                                    hitWall = true;
                                    game->bombs[i]->explosionPos.x += 64;
                                }
                            }
                        }
                    }
                    if (!hitWall && game->bombs[i]->explosionPos.x > 32) {
                        SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionMiddle, NULL, &game->bombs[i]->explosionPos, 180, NULL, SDL_FLIP_NONE);
                    }
                }
                game->bombs[i]->explosionPos.x -= 64;
                if (!stoneWall && game->bombs[i]->explosionPos.x > 32) {
                    SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionEnd, NULL, &game->bombs[i]->explosionPos, 180, NULL, SDL_FLIP_NONE);
                }
                game->bombs[i]->explosionPos.x = game->bombs[i]->bombPos.x - 7;                                                     // Återställer startvärden
                stoneWall = false;
                hitWall = false;

                // Renderar explosion uppåt
                for (int j = 0; j < range - 1; j++) {
                    game->bombs[i]->explosionPos.y -= 64;

                    for (int row = 0; row < ROW_SIZE; row++) {
                        for (int column = 0; column < COLUMN_SIZE; column++) {
                            if (activeBox[row][column] > 0) {

                                int boxLeft = column * 64 + 64;
                                int boxRight = boxLeft + 64;
                                int boxUp = row * 64 + 64;
                                int boxDown = boxUp + 64;

                                if ((game->bombs[i]->explosionPos.x + 32) > boxLeft && (game->bombs[i]->explosionPos.x + 32) < boxRight && (game->bombs[i]->explosionPos.y + 32) > boxUp
                                    && (game->bombs[i]->explosionPos.y) + 32 < boxDown) {

                                    if (activeBox[row][column] == 3) {
                                        stoneWall = true;
                                    }

                                    row = ROW_SIZE;
                                    column = COLUMN_SIZE;
                                    j = range;
                                    hitWall = true;
                                    game->bombs[i]->explosionPos.y += 64;
                                }
                            }
                        }
                    }
                    if (!hitWall && game->bombs[i]->explosionPos.y > 32) {
                        SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionMiddle, NULL, &game->bombs[i]->explosionPos, 270, NULL, SDL_FLIP_NONE);
                    }
                }
                game->bombs[i]->explosionPos.y -= 64;
                if (!stoneWall && game->bombs[i]->explosionPos.y > 32) {
                    SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionEnd, NULL, &game->bombs[i]->explosionPos, 270, NULL, SDL_FLIP_NONE);
                }
                game->bombs[i]->explosionPos.y = game->bombs[i]->bombPos.y - 7;                                                     // Återställer startvärden
                stoneWall = false;
                hitWall = false;

                // Renderar explosion nedåt
                for (int j = 0; j < range - 1; j++) {
                    game->bombs[i]->explosionPos.y += 64;

                    for (int row = 0; row < ROW_SIZE; row++) {
                        for (int column = 0; column < COLUMN_SIZE; column++) {
                            if (activeBox[row][column] > 0) {

                                int boxLeft = column * 64 + 64;
                                int boxRight = boxLeft + 64;
                                int boxUp = row * 64 + 64;
                                int boxDown = boxUp + 64;

                                if ((game->bombs[i]->explosionPos.x + 32) > boxLeft && (game->bombs[i]->explosionPos.x + 32) < boxRight && (game->bombs[i]->explosionPos.y + 32) > boxUp
                                    && (game->bombs[i]->explosionPos.y) + 32 < boxDown) {

                                    if (activeBox[row][column] == 3) {
                                        stoneWall = true;
                                    }
                                    row = ROW_SIZE;
                                    column = COLUMN_SIZE;
                                    j = range;
                                    hitWall = true;
                                    game->bombs[i]->explosionPos.y -= 64;
                                }
                            }
                        }
                    }
                    if (!hitWall && game->bombs[i]->explosionPos.y < WINDOW_HEIGHT - 92) {
                        SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionMiddle, NULL, &game->bombs[i]->explosionPos, 90, NULL, SDL_FLIP_NONE);
                    }
                }
                game->bombs[i]->explosionPos.y += 64;
                if (!stoneWall && game->bombs[i]->explosionPos.y < WINDOW_HEIGHT - 92) {
                    SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionEnd, NULL, &game->bombs[i]->explosionPos, 90, NULL, SDL_FLIP_NONE);
                }
                game->bombs[i]->explosionPos.y = game->bombs[i]->bombPos.y - 7;                                                     // Återställer startvärden
                stoneWall = false;
                hitWall = false;
            }
            if (game->bombs[i]->endExplosion) {
                game->bombs[i] = NULL;                                                                          // Raderar bomben
                // printf("Explosion timer done. Bomb deleted.\n");
            } 
        }
    }
}

PRIVATE Bomb createBomb(int playerPosX, int playerPosY, SDL_Renderer *renderer) {
    Bomb bomb = malloc(sizeof(struct BombSettings));

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
    bomb->explosionRange = 4;

    bomb->explosionHor.x = bomb->bombPos.x - 7 - (64 * (bomb->explosionRange));   // Start horizontal explosion to the left of bomb
    bomb->explosionHor.y = bomb->bombPos.y - 7;
    bomb->explosionHor.w = 64 * bomb->explosionRange * 2 + 64;
    bomb->explosionHor.h = 64;
    bomb->explosionVer.x = bomb->bombPos.x - 7;
    bomb->explosionVer.y = bomb->bombPos.y - 7 - (64 * (bomb->explosionRange));   // Start vertical explosion above bomb
    bomb->explosionVer.w = 64;
    bomb->explosionVer.h = bomb->explosionHor.w;

    bomb->switchRedBomb = false;                                                  // R�d bomb avaktiverad fr�n b�rjan
    bomb->startExplosion = false;
    bomb->endExplosion = false;
    bomb->spawnInside = true;

    return bomb;
}

PRIVATE void handleExplosions(Bomb bomb) {
    uint8_t col = bomb->explosionPos.x / 64 - 1;
    uint8_t row = bomb->explosionPos.y / 64 - 1;
    uint8_t i;

    int xStart = bomb->explosionHor.x;
    int xOffset = 0;
    int yStart = bomb->explosionVer.y;
    int yOffset = 0;

    // Adjust explosion size if box or wall exists on the left part of the explosion
    for (i = 0; i < bomb->explosionRange && col - i - 1 >= 0; i++) {
        if (activeBox[row][col - i - 1] == W) {
            bomb->explosionHor.x += 64 * (bomb->explosionRange - i);
            xOffset = bomb->explosionHor.x - xStart;          // Stores offset that is used if horizontal width is changed as well as horizontal x
            break;
        }
        else if (activeBox[row][col - i - 1] == 1) {
            bomb->explosionHor.x += 64 * (bomb->explosionRange - i) - 64;
            xOffset = bomb->explosionHor.x - xStart;
            activeBox[row][col - i - 1] = 0;                  // Deletes box
            break;
        }
    }

    // Adjust explosion size if box or wall exists on the right part of the explosion
    for (i = 0; i < bomb->explosionRange && col + i + 1 < COLUMN_SIZE; i++) {
        if (activeBox[row][col + i + 1] == W) {
            bomb->explosionHor.w -= 64 * (bomb->explosionRange - i) + xOffset;  // xOffset is used to modify the width in cases when      
            break;                                                              // a wall or box exists on the left part of the explosion
        }
        else if (activeBox[row][col + i + 1] == 1) {
            bomb->explosionHor.w -= 64 * (bomb->explosionRange - i) - 64 + xOffset;
            activeBox[row][col + i + 1] = 0;
            break;
        }
    }

    // Adjust explosion size if box or wall exists on the top part of the explosion
    for (i = 0; i < bomb->explosionRange && row - i - 1 >= 0; i++) {
        if (activeBox[row - i - 1][col] == W) {
            bomb->explosionVer.y += 64 * (bomb->explosionRange - i);
            yOffset = bomb->explosionVer.y - yStart;
            break;
        }
        else if (activeBox[row - i - 1][col] == 1) {
            bomb->explosionVer.y += 64 * (bomb->explosionRange - i) - 64;
            yOffset = bomb->explosionVer.y - yStart;
            activeBox[row - i - 1][col] = 0;
            break;
        }
    }

    // Adjust explosion size if box or wall exists on the bottom part of the explosion
    for (i = 0; i < bomb->explosionRange && row + i + 1 < ROW_SIZE; i++) {
        if (activeBox[row + i + 1][col] == W) {
            bomb->explosionVer.h -= 64 * (bomb->explosionRange - i) + yOffset;
            break;
        }
        else if (activeBox[row + i + 1][col] == 1) {
            bomb->explosionVer.h -= 64 * (bomb->explosionRange - i) - 64 + yOffset;
            activeBox[row + i + 1][col] = 0;
            break;
        }
    }
}

PUBLIC void initBombs(Bomb bombs[]) {
    for (int i = 0; i < BOMBS; i++) {
        bombs[i] = NULL;
    }
}