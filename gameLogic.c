#include "gameLogic.h"

#define SCREENMAX_X 970
#define SCREENMIN_X 48
#define SCREENMAX_Y 700
#define SCREENMIN_Y 48

#define CHAR_REFRESH_RATE 20                // Higher value = Slower animation transition

#define BOMB_SIZE 34

typedef struct InvincibilityCallbackArgs {
    Player player;
    udpData packetData;
    Network net;
} InvincibilityCallbackArgs;

Uint32 disableInvincibility(Uint32 interval, void *args);

void handlePlayerExplosionCollision(Game game, Network net, udpData packetData) {
    if (!playerIsHurt(game->player[game->pIdx])) { 

        for (uint8_t i = 0; i < BOMBS; i++) {
            if (game->bombs[i] != NULL) {
                modifyHitboxPos(game->player[game->pIdx]);

                if (game->bombs[i]->startExplosion == true && game->bombs[i]->endExplosion == false) {
                    if (SDL_HasIntersection(getPlayerHitboxPos(game->player[game->pIdx]), &game->bombs[i]->explosionHor) || SDL_HasIntersection(getPlayerHitboxPos(game->player[game->pIdx]), &game->bombs[i]->explosionVer)) {
                        setPlayerToHurt(game->player[game->pIdx]);
                        packetData->isHurt = 1;
                        net->willSend = true;

                        InvincibilityCallbackArgs *callbackArgs = malloc(sizeof(InvincibilityCallbackArgs));
                        callbackArgs->player = game->player[game->pIdx];
                        callbackArgs->net = net;
                        callbackArgs->packetData = packetData;

                        decrementPlayerLives(game->player[game->pIdx]);
                        if (!playerHasLivesRemaining(game->player[game->pIdx])) {
                            setPlayerToDead(game->player[game->pIdx]);
                            printf("Player died.\n");
                            
                            game->playersDead++;
                            
                            if (game->activePlayers == game->playersDead) {
                                game->allPlayersDead = true;
                                setToLastPlayer(game->player[game->pIdx]);
                            }

                            packetData->isDead = 1;
                            return;
                        }

                        game->invincibleTimer = SDL_AddTimer(2000, disableInvincibility, callbackArgs);
                    }
                }  
            } 
        }
    }
}

Uint32 disableInvincibility(Uint32 interval, void *args) {
    InvincibilityCallbackArgs *cArgs = (InvincibilityCallbackArgs*) args;
    setPlayerToNotHurt(cArgs->player);
    cArgs->packetData->isHurt = 0;
    cArgs->net->willSend = true;

    free(cArgs);
    return 0;
}

bool checkCollision(Player player, Bomb bombs[]) {
    if (!collisionMap(player))
        return false; 
    if (!collisionBoxes(player))
        return false; 
    if (!collisionBomb(player, bombs))
        return false;

    return true; 
}

bool collisionMap(Player player) {
    if (getPlayerPosY(player) < SCREENMIN_Y)
    {
        return false; 
    }

    if (getPlayerPosY(player) > SCREENMAX_Y)
    {
        return false;
    }

    if (getPlayerPosX(player) < SCREENMIN_X)
    {
        return false;
    }

    if (getPlayerPosX(player) > SCREENMAX_X)
    {
        return false;
    }
    return true;
}

bool collisionBoxes(Player player)
{
    int posBoxX = 0, posBoxY = 0;
    int sizeBox = 32;
    int sizePlayer = 32;
    const int botPlayerBigger = 16;

    for (int row = 0; row < ROW_SIZE; row++) {
        for (int column = 0; column < COLUMN_SIZE; column++) {
            if (activeBox[row][column] == 1 || activeBox[row][column] == 3)
            {
                posBoxX = column * 64 + 64;
                posBoxY = row * 64 + 64;

                if (!(getPlayerPosX(player) > posBoxX + sizeBox || posBoxX > getPlayerPosX(player) + sizePlayer || getPlayerPosY(player) > posBoxY + sizeBox ||
                    getPlayerPosY(player) + botPlayerBigger + sizePlayer < posBoxY)) {
                    return false;
                }
            }
        }
    }
    return true;
}


void pickUpPowerUps(Player player, Network net, udpData packetData) {
    for (int row = 0; row < ROW_SIZE; row++) {
        for (int column = 0; column < COLUMN_SIZE; column++) {
            if (activePowers[row][column] >= 4) {
                int powerUpLeft = column * 64 + 64 - 30;
                int powerUpRight = powerUpLeft + 64;
                int powerUpUp = row * 64 + 14;
                int powerUpDown = powerUpUp + 70;

                if (getPlayerPosX(player) > powerUpLeft && getPlayerPosX(player) < powerUpRight && getPlayerPosY(player) > powerUpUp && getPlayerPosY(player) < powerUpDown)
                {
                    if (activePowers[row][column] == 4) {
                        printf("Picked up power-up: Speed\n");
                        if (getPlayerSpeed(player) < 6) {  // Speed värdet startar 2, max 3 uppgraderingar
                            incrementPlayerSpeed(player);
                            printf("Speed increased!\n");
                        }
                    }
                    else if (activePowers[row][column] == 5) {
                        printf("Picked up power-up: +1 Bombs\n");
                        if (getMaxBombs(player) < 5) {				// Max 5 bomber, (plockar up max 4 uppgraderingar)
                            incrementBombsAvailable(player);
                            incrementMaxBombs(player);
                            printf("+1 Bombs!\n");
                        }
                    }
                    else if (activePowers[row][column] == 6) {
                        printf("Picked up power-up: Longer Explosion\n");
                        if (getPlayerExpRange(player) < 5) {				// Max längd = mitten rutan + 5 rutor ut
                            incrementPlayerExpRange(player);
                            printf("Explosion Range increased!\n");
                            packetData->explosionRange = getPlayerExpRange(player);
                        }
                    }
                    packetData->powerUpRow = row;
                    packetData->powerUpCol = column;
                    packetData->powerUpTaken = 1;
                    powerUpGone(row, column, 0);
                    net->willSend = true;
                }
            }
        }
    }
}


bool collisionBomb(Player player, Bomb bombs[]) {
    for (uint8_t i = 0; i < BOMBS; i++) {
        if (bombs[i] != NULL) {
            int left = (bombs[i]->bombPos.x) - 5;
            int right = (bombs[i]->bombPos.x) + ((64 - BOMB_SIZE) / 2 + 10);
            int up = (bombs[i]->bombPos.y) - 10;
            int down = (bombs[i]->bombPos.y) + ((64 - BOMB_SIZE) / 2 + 10);

            if (bombs[i]->spawnInside) {
                if (!(getPlayerPosX(player) > right || left > getPlayerPosX(player) + BOMB_SIZE || getPlayerPosY(player) > down || getPlayerPosY(player) + BOMB_SIZE < up))
                {
                    return true;
                }
                else
                {
                    bombs[i]->spawnInside = false;
                    return true;
                }
            }

            if (!(getPlayerPosX(player) > right || left > getPlayerPosX(player) + BOMB_SIZE || getPlayerPosY(player) > down || getPlayerPosY(player) + BOMB_SIZE < up))
            {
                return false;
            }
        }
    }
    return true;
}

void powerUpGone(int row, int col, int value) {
    activePowers[row][col] = value;
}

void move(Player player, int *lastMove, int *newMove, char key, Bomb bombs[], int *frames, Network net, udpData packetData) {
    static int netDelay = 0;

    switch (key) {
    case 's':
        increasePlayerPosY(player);
        if (!checkCollision(player, bombs)) {
            decreasePlayerPosY(player);
        }
        if (*newMove == *lastMove && getPlayerFrame(player) <= 3) {
            *frames += 1;
            if (*frames == CHAR_REFRESH_RATE / getPlayerSpeed(player)) {
                *frames = 0;
                *lastMove = *newMove;
                incrementPlayerFrame(player);
                if (getPlayerFrame(player) > 3) {
                    setPlayerFrame(player, 0);
                }
            }
        }
        else {
            *frames = 0;
            setPlayerFrame(player, 0);
            *lastMove = *newMove = 0;
        }
        break;

    case 'w':
        decreasePlayerPosY(player);
        if (!checkCollision(player, bombs)) {
            increasePlayerPosY(player);
        }
        if (*newMove == *lastMove && getPlayerFrame(player) <= 7 && getPlayerFrame(player) > 3) {
            *frames += 1;
            if (*frames == CHAR_REFRESH_RATE / getPlayerSpeed(player)) {
                *frames = 0;
                *lastMove = *newMove;
                incrementPlayerFrame(player);
                if (getPlayerFrame(player) > 7) {
                    setPlayerFrame(player, 4);
                }
            }
        }
        else {
            *frames = 0;
            setPlayerFrame(player, 4);
            *lastMove = *newMove = 4;
        }
        break;

    case 'a':
        decreasePlayerPosX(player);
        if (!checkCollision(player, bombs)) {
            increasePlayerPosX(player);
        }
        if (*newMove == *lastMove && getPlayerFrame(player) <= 15 && getPlayerFrame(player) > 11) {
            *frames += 1;
            if (*frames == CHAR_REFRESH_RATE / getPlayerSpeed(player)) {
                *frames = 0;
                *lastMove = *newMove;
                incrementPlayerFrame(player);
                if (getPlayerFrame(player) > 15) {
                    setPlayerFrame(player, 12);
                }
            }
        }
        else {
            *frames = 0;
            setPlayerFrame(player, 12);
            *lastMove = *newMove = 12;
        }
        break;

    case 'd':
        increasePlayerPosX(player);
        if (!checkCollision(player, bombs)) {
            decreasePlayerPosX(player);
        }
        if (*newMove == *lastMove && getPlayerFrame(player) <= 11 && getPlayerFrame(player) > 7) {
            *frames += 1;
            if (*frames == CHAR_REFRESH_RATE / getPlayerSpeed(player)) {
                *frames = 0;
                *lastMove = *newMove;
                incrementPlayerFrame(player);
                if (getPlayerFrame(player) > 11) {
                    setPlayerFrame(player, 8);
                }
            }
        }
        else {
            *frames = 0;
            setPlayerFrame(player, 8);
            *lastMove = *newMove = 8;
        }
        break;

    default: break;
    }

    if (netDelay < 1) {                             // 1 = 50%, 2 = 66%, 3 = 75%, 4 = 80%, 5 = 83%...
        netDelay++;                                 // Till exempel om 5: kör på 0-4 och skippa 5. Alltså kör 5 frames och skippa frame 6
        packetData->xPos = getPlayerPosX(player);
        packetData->yPos = getPlayerPosY(player);
        packetData->frame = getPlayerFrame(player);
        net->willSend = true;
    }
    else {
        netDelay = 0;
    }
}