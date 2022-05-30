#include "bomb.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define BOMB_WIDTH 50
#define BOMB_HEIGHT 50

PRIVATE Uint32 explosionDone(Uint32 interval, void *deleteBomb);
PRIVATE void handleExplosions(Bomb bomb, Network net, uint8_t pIdxSelf, udpData packetData,Boxes boxes);
PUBLIC void bombPlacement(Player player, Bomb bombs[], uint8_t pIdx, SDL_Renderer *renderer, Network net, udpData packetData,Boxes boxes);
PUBLIC void renderBombsAndExplosions(Game game, Network net, udpData packetData,Boxes boxes);

PUBLIC void bombPlacement(Player player, Bomb bombs[], uint8_t pIdx, SDL_Renderer *renderer, Network net, udpData packetData,Boxes boxes) {
    //IF PLAYER HAVE BOMBS AVAILABLE // 
    if (bombsAvailable(player))  {
        uint8_t bombIdx = getBombIdx(bombs);             // Get first free index to store bomb
        decrementBombsAvailable(player);

        BombTimerCallbackArgs *callbackArgs = malloc(sizeof(BombTimerCallbackArgs));
        // CREATES THE BOMB //
        callbackArgs->bomb = bombs[bombIdx] = createBomb(getPlayerPosX(player), getPlayerPosY(player), pIdx, renderer, getPlayerExpRange(player));
        callbackArgs->bombsAvailable = getBombsAvailableVal(player);
        callbackArgs->pIdx = pIdx;
        callbackArgs->packetData = packetData;
        callbackArgs->net = net;
        callbackArgs->boxes = boxes;

        packetData->bombDropped = 1;
        packetData->bombPosX = bombs[bombIdx]->bombPos.x;
        packetData->bombPosY = bombs[bombIdx]->bombPos.y;
        net->willSend = 1;

        bombs[bombIdx]->redBombTime = SDL_AddTimer(2000, redBomb, callbackArgs);                     // Timer until red bomb shows
        bombs[bombIdx]->bombTime = SDL_AddTimer(3000, explodeBomb, callbackArgs);                    // Timer until explosion
        bombs[bombIdx]->deleteBombTime = SDL_AddTimer(4000, explosionDone, callbackArgs);          
    }
}

/* GET INDEX OF A BOMB IN THE BOMBS ARRAY */

PUBLIC uint8_t getBombIdx(Bomb bombs[]) {
    for (uint8_t i = BOMBS-1; i > 0; i--) {
        if (bombs[i] == NULL) {
            return i;
        }
    }
    return 0;
}

/* MAKES BOMB RED FOR 2 s */

PUBLIC Uint32 redBomb(Uint32 interval, void *args) {
    BombTimerCallbackArgs* bargs = (BombTimerCallbackArgs*) args;
    bargs->bomb->switchRedBomb = true;

    return 0;
}


/* Starts the explosion */
PUBLIC Uint32 explodeBomb(Uint32 interval, void *args) {
    BombTimerCallbackArgs* bargs = (BombTimerCallbackArgs*) args;
    bargs->bomb->startExplosion = true;

    handleExplosions(bargs->bomb, bargs->net, bargs->pIdx, bargs->packetData,bargs->boxes);    // Modifies explosion hitbox based on collision with walls and boxes as well as deletes boxes

    return 0;
}

PRIVATE Uint32 explosionDone(Uint32 interval, void *args) {
    BombTimerCallbackArgs *bargs = (BombTimerCallbackArgs*) args;
    bargs->bomb->endExplosion = true;
    (*(bargs->bombsAvailable))++;

    free(bargs);
    return 0;
}

PUBLIC Uint32 explosionDoneClient(Uint32 interval, void *args) {
    BombTimerCallbackArgs *bargs = (BombTimerCallbackArgs*) args;
    bargs->bomb->endExplosion = true;

    free(bargs);
    return 0;
}

PUBLIC void renderBombsAndExplosions(Game game, Network net, udpData packetData,Boxes boxes) {
    for (uint8_t i = 0; i < BOMBS; i++) {
        if (game->bombs[i] != NULL) {
            if (game->bombs[i]->switchRedBomb) {
                SDL_RenderCopy(game->renderer, game->bombs[i]->textureBombRed, NULL, &game->bombs[i]->bombPos);     // Renderer red bomb 
            }
            else {
                SDL_RenderCopy(game->renderer, game->bombs[i]->textureBomb, NULL, &game->bombs[i]->bombPos);        // Renderar black bomb
            }
            bool EraseBoxTopRight = false;
            int rightBoxRow;
            int rightBoxColumn;
            bool EraseBoxTopLeft = false;
            int leftBoxRow;
            int leftBoxColumn;
            bool EraseBoxTopTop = false;
            int upBoxRow;
            int upBoxColumn;
            bool createPowerUpDown = false;
            int downBoxRow;
            int downBoxColumn;

            // If startExplosion is true did bomb hit player, box or wall
            if (game->bombs[i]->startExplosion) {

                int range = game->bombs[i]->explosionRange;
                bool stoneWall = false;
                bool hitWall = false;

                SDL_RenderCopy(game->renderer, game->bombs[i]->textureExplosionStart, NULL, &game->bombs[i]->explosionPos);         // Renderer start of explosion

                // Rendere right explosion
                for (int j = 0; j < range; j++) {
                    game->bombs[i]->explosionPos.x += 64;

                    for (int row = 0; row < ROW_SIZE; row++) {
                        for (int column = 0; column < COLUMN_SIZE; column++) {
                            if (getActiveBox(boxes,row,column) == 1 || getActiveBox(boxes, row, column) == W) {


                                int boxLeft = column * 64 + 64;
                                int boxRight = boxLeft + 64;
                                int boxUp = row * 64 + 64;
                                int boxDown = boxUp + 64;

                                // IF HIT STONEWALL OR BOX//
                                if ((game->bombs[i]->explosionPos.x + 32) > boxLeft && (game->bombs[i]->explosionPos.x + 32) < boxRight && (game->bombs[i]->explosionPos.y + 32) > boxUp
                                    && (game->bombs[i]->explosionPos.y) + 32 < boxDown) {
                                    if (getActiveBox(boxes, row, column) == W) {
                                        stoneWall = true;
                                    }
                                    else if (getActiveBox(boxes, row, column) == 1) {
                                        EraseBoxTopRight = true;
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
                        if (j == range - 1) {
                            game->bombs[i]->explosionPos.x -= 64;
                        }
                        else {
                            SDL_RenderCopy(game->renderer, game->bombs[i]->textureExplosionMiddle, NULL, &game->bombs[i]->explosionPos);
                        }
                    }
                }
                game->bombs[i]->explosionPos.x += 64;
                if (!stoneWall && game->bombs[i]->explosionPos.x < WINDOW_WIDTH - 92) {
                    SDL_RenderCopy(game->renderer, game->bombs[i]->textureExplosionEnd, NULL, &game->bombs[i]->explosionPos);
                }
                if (EraseBoxTopRight) {
                    rightBoxRow = game->bombs[i]->explosionPos.y / 64 - 1;
                    rightBoxColumn = game->bombs[i]->explosionPos.x / 64 - 1;
                }
                game->bombs[i]->explosionPos.x = game->bombs[i]->bombPos.x - 7;                                                     // Restore start value
                stoneWall = false;
                hitWall = false;

                // Rendere explosion left
                for (int j = 0; j < range; j++) {
                    game->bombs[i]->explosionPos.x -= 64;

                    for (int row = 0; row < ROW_SIZE; row++) {
                        for (int column = 0; column < COLUMN_SIZE; column++) {
                            if (getActiveBox(boxes, row, column) == 1 || getActiveBox(boxes, row, column) == W) {

                                int boxLeft = column * 64 + 64;
                                int boxRight = boxLeft + 64;
                                int boxUp = row * 64 + 64;
                                int boxDown = boxUp + 64;

                                if ((game->bombs[i]->explosionPos.x + 32) > boxLeft && (game->bombs[i]->explosionPos.x + 32) < boxRight && (game->bombs[i]->explosionPos.y + 32) > boxUp
                                    && (game->bombs[i]->explosionPos.y) + 32 < boxDown) {

                                    if (getActiveBox(boxes, row, column) == W) {
                                        stoneWall = true;
                                    }
                                    else if (getActiveBox(boxes, row, column) == 1) {
                                        EraseBoxTopLeft = true;
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
                        if (j == range - 1) {
                            game->bombs[i]->explosionPos.x += 64;
                        }
                        else {
                            SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionMiddle, NULL, &game->bombs[i]->explosionPos, 180, NULL, SDL_FLIP_NONE);
                        }
                    }
                }
                game->bombs[i]->explosionPos.x -= 64;
                if (!stoneWall && game->bombs[i]->explosionPos.x > 32) {
                    SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionEnd, NULL, &game->bombs[i]->explosionPos, 180, NULL, SDL_FLIP_NONE);
                }
                if (EraseBoxTopLeft) {
                    leftBoxRow = game->bombs[i]->explosionPos.y / 64 - 1;
                    leftBoxColumn = game->bombs[i]->explosionPos.x / 64 - 1;
                }
                game->bombs[i]->explosionPos.x = game->bombs[i]->bombPos.x - 7;                                                     // Restores start value
                stoneWall = false;
                hitWall = false;

                // Rendere explosion top
                for (int j = 0; j < range; j++) {
                    game->bombs[i]->explosionPos.y -= 64;

                    for (int row = 0; row < ROW_SIZE; row++) {
                        for (int column = 0; column < COLUMN_SIZE; column++) {
                            if (getActiveBox(boxes, row, column) == 1 || getActiveBox(boxes, row, column) == W) {

                                int boxLeft = column * 64 + 64;
                                int boxRight = boxLeft + 64;
                                int boxUp = row * 64 + 64;
                                int boxDown = boxUp + 64;

                                if ((game->bombs[i]->explosionPos.x + 32) > boxLeft && (game->bombs[i]->explosionPos.x + 32) < boxRight && (game->bombs[i]->explosionPos.y + 32) > boxUp
                                    && (game->bombs[i]->explosionPos.y) + 32 < boxDown) {

                                    if (getActiveBox(boxes, row, column) == W) {
                                        stoneWall = true;
                                    }
                                    else if (getActiveBox(boxes, row, column) == 1) {
                                        EraseBoxTopTop = true;
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
                        if (j == range - 1) {
                            game->bombs[i]->explosionPos.y += 64;
                        }
                        else {
                            SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionMiddle, NULL, &game->bombs[i]->explosionPos, 270, NULL, SDL_FLIP_NONE);
                        }
                    }
                }
                game->bombs[i]->explosionPos.y -= 64;
                if (!stoneWall && game->bombs[i]->explosionPos.y > 32) {
                    SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionEnd, NULL, &game->bombs[i]->explosionPos, 270, NULL, SDL_FLIP_NONE);
                }
                if (EraseBoxTopTop) {
                    upBoxRow = game->bombs[i]->explosionPos.y / 64 - 1;
                    upBoxColumn = game->bombs[i]->explosionPos.x / 64 - 1;
                }
                game->bombs[i]->explosionPos.y = game->bombs[i]->bombPos.y - 7;                                                      // Restores start value
                stoneWall = false;
                hitWall = false;

                // Renderer explosion bot
                for (int j = 0; j < range; j++) {
                    game->bombs[i]->explosionPos.y += 64;

                    for (int row = 0; row < ROW_SIZE; row++) {
                        for (int column = 0; column < COLUMN_SIZE; column++) {
                            if (getActiveBox(boxes, row, column) == 1 || getActiveBox(boxes, row, column) == W) {

                                int boxLeft = column * 64 + 64;
                                int boxRight = boxLeft + 64;
                                int boxUp = row * 64 + 64;
                                int boxDown = boxUp + 64;

                                if ((game->bombs[i]->explosionPos.x + 32) > boxLeft && (game->bombs[i]->explosionPos.x + 32) < boxRight && (game->bombs[i]->explosionPos.y + 32) > boxUp
                                    && (game->bombs[i]->explosionPos.y) + 32 < boxDown) {

                                    if (getActiveBox(boxes, row, column) == W) {
                                        stoneWall = true;
                                    }
                                    else if (getActiveBox(boxes, row, column) == 1) {
                                        createPowerUpDown = true;
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
                        if (j == range - 1) {
                            game->bombs[i]->explosionPos.y -= 64;
                        }
                        else {
                            SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionMiddle, NULL, &game->bombs[i]->explosionPos, 90, NULL, SDL_FLIP_NONE);
                        }
                    }
                }
                game->bombs[i]->explosionPos.y += 64;
                if (!stoneWall && game->bombs[i]->explosionPos.y < WINDOW_HEIGHT - 92) {
                    SDL_RenderCopyEx(game->renderer, game->bombs[i]->textureExplosionEnd, NULL, &game->bombs[i]->explosionPos, 90, NULL, SDL_FLIP_NONE);
                }
                if (createPowerUpDown) {
                    downBoxRow = game->bombs[i]->explosionPos.y / 64 - 1;
                    downBoxColumn = game->bombs[i]->explosionPos.x / 64 - 1;
                }
                game->bombs[i]->explosionPos.y = game->bombs[i]->bombPos.y - 7;                                                     // Restores start value
                stoneWall = false;
                hitWall = false;
            }
                    // ERASE BOXES // 
            if (game->bombs[i]->endExplosion) {
                if (EraseBoxTopRight) {
                    setBoxValue(boxes, rightBoxRow, rightBoxColumn); 
                }
                if (EraseBoxTopLeft) {
                    setBoxValue(boxes, leftBoxRow, leftBoxColumn);
                }
                if (EraseBoxTopTop) {
                    setBoxValue(boxes, upBoxRow, upBoxColumn);
                }
                if (createPowerUpDown) {
                    setBoxValue(boxes, downBoxRow, downBoxColumn);
                }
                free(game->bombs[i]);
                game->bombs[i] = NULL;   // ERASE BOMB
            } 
        }
    }
}


/* initialize datamembers in bomb struct */

PUBLIC Bomb createBomb(int playerPosX, int playerPosY, uint8_t pIdx, SDL_Renderer *renderer, int explosionRange) {
    Bomb bomb = malloc(sizeof(struct BombSettings));

    char pictureDestination[64];
    // LOADS NECESSARY IMAGES FOR BOMB // 
    SDL_strlcpy(pictureDestination, "resources/bomb.png", sizeof pictureDestination);
    loadTextures(&renderer, &bomb->surface, &bomb->textureBomb, pictureDestination);

    SDL_strlcpy(pictureDestination, "resources/bomb-red.png", sizeof pictureDestination);
    loadTextures(&renderer, &bomb->surface, &bomb->textureBombRed, pictureDestination);

    SDL_strlcpy(pictureDestination, "resources/explosion-start.png", sizeof pictureDestination);
    loadTextures(&renderer, &bomb->surface, &bomb->textureExplosionStart, pictureDestination);

    SDL_strlcpy(pictureDestination, "resources/explosion-middle.png", sizeof pictureDestination);
    loadTextures(&renderer, &bomb->surface, &bomb->textureExplosionMiddle, pictureDestination);

    SDL_strlcpy(pictureDestination, "resources/explosion-end.png", sizeof pictureDestination);
    loadTextures(&renderer, &bomb->surface, &bomb->textureExplosionEnd, pictureDestination);

    bomb->bombPos.x = ((playerPosX + 32) / 64) * 64 + 7;       // The bomb placement will be decided upon character placement  
    bomb->bombPos.y = ((playerPosY + 32) / 64) * 64 + 7;       // +7 bomb because bomb is 50x50 pixlar (64-50=7)
    bomb->bombPos.w = BOMB_WIDTH;
    bomb->bombPos.h = BOMB_HEIGHT;
    bomb->explosionPos.x = bomb->bombPos.x - 7;
    bomb->explosionPos.y = bomb->bombPos.y - 7;
    bomb->explosionPos.w = 64;
    bomb->explosionPos.h = 64;
    bomb->explosionRange = explosionRange;

    bomb->explosionHor.x = bomb->bombPos.x - 7 - (64 * (bomb->explosionRange));   // Start horizontal explosion to the left of bomb
    bomb->explosionHor.y = bomb->bombPos.y - 7;
    bomb->explosionHor.w = 64 * bomb->explosionRange * 2 + 64;
    bomb->explosionHor.h = 64;
    bomb->explosionVer.x = bomb->bombPos.x - 7;
    bomb->explosionVer.y = bomb->bombPos.y - 7 - (64 * (bomb->explosionRange));   // Start vertical explosion above bomb
    bomb->explosionVer.w = 64;
    bomb->explosionVer.h = bomb->explosionHor.w;

    bomb->switchRedBomb = false;                                                  // RED bomb is turned off in the begning
    bomb->startExplosion = false;
    bomb->endExplosion = false;
    bomb->spawnInside = true;
    bomb->pIdxWhoDroppedBomb = pIdx;

    return bomb;
}

PRIVATE void handleExplosions(Bomb bomb, Network net, uint8_t pIdxSelf, udpData packetData, Boxes boxes) {
    uint8_t col = bomb->explosionPos.x / 64 - 1;
    uint8_t row = bomb->explosionPos.y / 64 - 1;
    uint8_t i;

    int xStart = bomb->explosionHor.x;
    int xOffset = 0;
    int yStart = bomb->explosionVer.y;
    int yOffset = 0;

    // Adjust explosion size if box or wall exists on the left part of the explosion
    for (i = 0; i < bomb->explosionRange && col - i - 1 >= 0; i++) {
        if (getActiveBox(boxes,row, col - i - 1) == W) {
            bomb->explosionHor.x += 64 * (bomb->explosionRange - i);
            xOffset = bomb->explosionHor.x - xStart;          // Stores offset that is used if horizontal width is changed as well as horizontal x
            break;
        }
        else if (getActiveBox(boxes, row, col - i - 1) == 1) {

            bomb->explosionHor.x += 64 * (bomb->explosionRange - i) - 64;
            xOffset = bomb->explosionHor.x - xStart;
            
            if (bomb->pIdxWhoDroppedBomb == pIdxSelf) {
                activePowers[row][col - i - 1] = (rand() % 4) + 4;
                packetData->leftBoxVal = activePowers[row][col - i - 1];
                packetData->leftBoxRow = row;
                packetData->leftBoxCol = col - i - 1;
                net->willSend = true;
            }
            break;
        }
    }

    // Adjust explosion size if box or wall exists on the right part of the explosion
    for (i = 0; i < bomb->explosionRange && col + i + 1 < COLUMN_SIZE; i++) {
        if (getActiveBox(boxes,row, col + i + 1) == W) {
            bomb->explosionHor.w -= 64 * (bomb->explosionRange - i) + xOffset;  // xOffset is used to modify the width in cases when      
            break;                                                              // a wall or box exists on the left part of the explosion
        }
        else if (getActiveBox(boxes, row, col + i + 1) == 1) {
            bomb->explosionHor.w -= 64 * (bomb->explosionRange - i) - 64 + xOffset;

            if (bomb->pIdxWhoDroppedBomb == pIdxSelf) {
                activePowers[row][col + i + 1] = (rand() % 4) + 4;
                packetData->rightBoxVal = activePowers[row][col + i + 1];
                packetData->rightBoxRow = row;
                packetData->rightBoxCol = col + i + 1;
                net->willSend = true;
            }
            break;
        }
    }

    // Adjust explosion size if box or wall exists on the top part of the explosion
    for (i = 0; i < bomb->explosionRange && row - i - 1 >= 0; i++) {
        if (getActiveBox(boxes, row - i - 1, col) == W) {
            bomb->explosionVer.y += 64 * (bomb->explosionRange - i);
            yOffset = bomb->explosionVer.y - yStart;
            break;
        }
        else if (getActiveBox(boxes, row - i - 1, col) == W) {
            bomb->explosionVer.y += 64 * (bomb->explosionRange - i) - 64;
            yOffset = bomb->explosionVer.y - yStart;

            if (bomb->pIdxWhoDroppedBomb == pIdxSelf) {
                activePowers[row - i - 1][col] = (rand() % 4) + 4;
                packetData->topBoxVal = activePowers[row - i - 1][col];
                packetData->topBoxRow = row - i - 1;
                packetData->topBoxCol = col;
                net->willSend = true;
            }

            break;
        }
    }

    // Adjust explosion size if box or wall exists on the bottom part of the explosion
    for (i = 0; i < bomb->explosionRange && row + i + 1 < ROW_SIZE; i++) {
        if (getActiveBox(boxes, row + i + 1, col) == W) {
            bomb->explosionVer.h -= 64 * (bomb->explosionRange - i) + yOffset;
            break;
        }
        else if (getActiveBox(boxes, row + i + 1, col) == 1) {
            bomb->explosionVer.h -= 64 * (bomb->explosionRange - i) - 64 + yOffset;

            if (bomb->pIdxWhoDroppedBomb == pIdxSelf) {
                activePowers[row + i + 1][col] = (rand() % 4) + 4;
                packetData->bottomBoxVal = activePowers[row + i + 1][col];
                packetData->bottomBoxRow = row + i + 1;
                packetData->bottomBoxCol = col;
                net->willSend = true;
            }
            break;
        }
    }
}

PUBLIC void initBombs(Bomb bombs[]) {
    for (int i = 0; i < BOMBS; i++) {
        bombs[i] = NULL;
    }
}