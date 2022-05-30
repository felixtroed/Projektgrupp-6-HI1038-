#include "gameLogic.h"

#define PUBLIC /* empty */
#define PRIVATE static

#define SCREENMAX_X 970
#define SCREENMIN_X 48
#define SCREENMAX_Y 700
#define SCREENMIN_Y 48

#define CHAR_REFRESH_RATE 20                // Higher value = Slower animation transition

#define BOMB_SIZE 34

PRIVATE bool collisionBoxes(Player player, Boxes boxes);
PRIVATE void powerUpGone(int row, int col, int value);
PRIVATE bool checkCollision(Player player, Bomb bombs[], Boxes boxes);
PRIVATE Uint32 disableInvincibility(Uint32 interval, void* args);
PRIVATE bool checkCollision(Player player, Bomb bombs[], Boxes boxes);
PRIVATE bool collisionMap(Player player);
PRIVATE bool collisionBomb(Player player, Bomb bombs[]);

typedef struct InvincibilityCallbackArgs {
    Player player;
    udpData packetData;
    Network net;
} InvincibilityCallbackArgs;

Uint32 disableInvincibility(Uint32 interval, void *args);

PUBLIC void handlePlayerExplosionCollision(Game game, Network net, udpData packetData) {
   
    if (!playerIsHurt(game->player[game->pIdx]))  {  // wont load if player is hurt

        for (uint8_t i = 0; i < BOMBS; i++) {
            if (game->bombs[i] != NULL) {
                modifyHitboxPos(game->player[game->pIdx]); // moves character  +16 in x direction and +14 in y direction 

                if (game->bombs[i]->startExplosion == true && game->bombs[i]->endExplosion == false) {
                    if (SDL_HasIntersection(getPlayerHitboxPos(game->player[game->pIdx]), &game->bombs[i]->explosionHor) || SDL_HasIntersection(getPlayerHitboxPos(game->player[game->pIdx]), &game->bombs[i]->explosionVer)) {
                        setPlayerToHurt(game->player[game->pIdx]); // if an intersection occurs player takes damage 
                        packetData->isHurt = 1;
                        net->willSend = true;

                        InvincibilityCallbackArgs *callbackArgs = malloc(sizeof(InvincibilityCallbackArgs)); // makes space for the struct InvincibilityCallbackArgs
                        callbackArgs->player = game->player[game->pIdx]; 
                        callbackArgs->net = net;
                        callbackArgs->packetData = packetData;

                        decrementPlayerLives(game->player[game->pIdx]);
                        if (!playerHasLivesRemaining(game->player[game->pIdx])) {
                            setPlayerToDead(game->player[game->pIdx]);                            
                            game->playersDead++;
                            
                            if (game->activePlayers == game->playersDead) {
                                game->allPlayersDead = true; // if true a winner is decided 
                                setToLastPlayer(game->player[game->pIdx]);
                            }

                            packetData->isDead = 1;
                            return;
                        }

                        game->invincibleTimer = SDL_AddTimer(2000, disableInvincibility, callbackArgs); // makes a thread after 2 seconds to make player take damage again
                    }
                }  
            } 
        }
    }
}

PRIVATE Uint32 disableInvincibility(Uint32 interval, void *args) {
    InvincibilityCallbackArgs *cArgs = (InvincibilityCallbackArgs*) args;
    setPlayerToNotHurt(cArgs->player);
    cArgs->packetData->isHurt = 0;
    cArgs->net->willSend = true;
    free(cArgs);
    return 0;
}

PRIVATE bool checkCollision(Player player, Bomb bombs[],Boxes boxes) {
    if (!collisionMap(player))
        return false; 
    if (!collisionBoxes(player,boxes))
        return false; 
    if (!collisionBomb(player, bombs))
        return false;
    return true; 
}

PRIVATE bool collisionMap(Player player) {
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

PRIVATE bool collisionBoxes(Player player,Boxes boxes)
{
    int posBoxX = 0, posBoxY = 0;
    int sizeBox = 32;

    for (int row = 0; row < ROW_SIZE; row++) {
        for (int column = 0; column < COLUMN_SIZE; column++) {
            if (getActiveBox(boxes,row,column) == 1 || getActiveBox(boxes,row,column) == 3)
            {
                posBoxX = column * 64 + 64;
                posBoxY = row * 64 + 64;

                if (!(getPlayerPosX(player) > posBoxX + sizeBox || posBoxX > getPlayerPosX(player) + getPlayerWidth() || getPlayerPosY(player) > posBoxY + sizeBox ||
                    getPlayerPosY(player) + getPlayerHeight()< posBoxY)) {
                    return false;
                }
            }
        }
    }
    return true;
}


PRIVATE void pickUpPowerUps(Player player, Network net, udpData packetData) {
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
                        if (getPlayerSpeed(player) < 6) {  // Speed 
                            incrementPlayerSpeed(player);
                        }
                    }
                    else if (activePowers[row][column] == 5) {
                        if (getMaxBombs(player) < 5) {			// number off bombs
                            incrementBombsAvailable(player);
                            incrementMaxBombs(player);
                        }
                    }
                    else if (activePowers[row][column] == 6) {
                        if (getPlayerExpRange(player) < 5) {				// Max range = 5 squares
                            incrementPlayerExpRange(player);
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


PRIVATE bool collisionBomb(Player player, Bomb bombs[]) {
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

PRIVATE void powerUpGone(int row, int col, int value) {
    activePowers[row][col] = value;
}



void move(Player player, int *lastMove, int *newMove, char key, Bomb bombs[], int *frames, Network net, udpData packetData,Boxes boxes) {
    static int netDelay = 0;

    switch (key) {
    case 's':
        increasePlayerPosY(player); 
        pickUpPowerUps(player, net, packetData);
        if (!checkCollision(player, bombs,boxes)) {
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
        pickUpPowerUps(player, net, packetData);

        if (!checkCollision(player, bombs,boxes)) {
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
        pickUpPowerUps(player, net, packetData);

        if (!checkCollision(player, bombs,boxes)) {
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
        pickUpPowerUps(player, net, packetData);
        if (!checkCollision(player, bombs,boxes)) {
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
        netDelay++;                                 
        packetData->xPos = getPlayerPosX(player);
        packetData->yPos = getPlayerPosY(player);
        packetData->frame = getPlayerFrame(player);
        net->willSend = true;
    }
    else {
        netDelay = 0;
    }
}