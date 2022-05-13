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
    if (game->player[game->pIdx]->isHurt == false) { 

        for (uint8_t i = 0; i < BOMBS; i++) {
            if (game->bombs[i] != NULL) {
                game->player[game->pIdx]->hitboxPos.x = game->player[game->pIdx]->pos.x + 16;       // Puts hitbox in the same position as character
                game->player[game->pIdx]->hitboxPos.y = game->player[game->pIdx]->pos.y + 14;

                if (game->bombs[i]->startExplosion == true && game->bombs[i]->endExplosion == false) {
                    if (SDL_HasIntersection(&game->player[game->pIdx]->hitboxPos, &game->bombs[i]->explosionHor) || SDL_HasIntersection(&game->player[game->pIdx]->hitboxPos, &game->bombs[i]->explosionVer)) {
                        game->player[game->pIdx]->isHurt = true;
                        packetData->isHurt = 1;
                        net->willSend = true;

                        InvincibilityCallbackArgs *callbackArgs = malloc(sizeof(InvincibilityCallbackArgs));
                        callbackArgs->player = game->player[game->pIdx];
                        callbackArgs->net = net;
                        callbackArgs->packetData = packetData;
                        
                        (game->player[game->pIdx]->lifes)--;
                        if (game->player[game->pIdx]->lifes <= 0) {
                            game->player[game->pIdx]->isAlive = false;
                            printf("Player died.\n");
                            packetData->isDead = 1;
                            game->playersDead++;
                            return;
                        }

                        game->player[game->pIdx]->invincibleTimer = SDL_AddTimer(2000, disableInvincibility, callbackArgs);
                    }
                }  
            } 
        }
    }
}

Uint32 disableInvincibility(Uint32 interval, void *args) {
    InvincibilityCallbackArgs *cArgs = (InvincibilityCallbackArgs*) args;
    cArgs->player->isHurt = false;
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
    if (player->pos.y < SCREENMIN_Y)
    {
        return false; 
    }

    if (player->pos.y > SCREENMAX_Y)
    {
        return false;
    }

    if (player->pos.x < SCREENMIN_X)
    {
        return false;
    }

    if (player->pos.x > SCREENMAX_X)
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

                if (!(player->pos.x > posBoxX + sizeBox || posBoxX > player->pos.x + sizePlayer || player->pos.y > posBoxY + sizeBox ||
                    player->pos.y + botPlayerBigger + sizePlayer < posBoxY))
                {
                    // printf("ROW %d COL %d \n", row, column);
                    // printf("POS X: %d POS Y %d \n", p1->pos.x, p1->pos.y);
                    // printf("BOX: POS X: %d POS Y:%d\n ", posBoxX, posBoxY);
                    return false;

                }
            }
        }
    }
    return true;
}


void pickUpPowerUps(Player player,Network net, udpData packetData) {
    //printf("char posX: %d\tposY: %d\n", p1->pos.x, p1->pos.y);
    for (int row = 0; row < ROW_SIZE; row++) {
        for (int column = 0; column < COLUMN_SIZE; column++) {
            if (activePowers[row][column] >= 4) {
                int powerUpLeft = column * 64 + 64 - 30;
                int powerUpRight = powerUpLeft + 64;
                int powerUpUp = row * 64 + 14;
                int powerUpDown = powerUpUp + 70;

                if (player->pos.x > powerUpLeft && player->pos.x < powerUpRight && player->pos.y > powerUpUp && player->pos.y < powerUpDown)
                {
                    if (activePowers[row][column] == 4) {
                        printf("Picked up power-up: Speed\n");
                        if (player->speed < 5) {				// Speed värdet startar 2, max 3 uppgraderingar
                            player->speed++;
                            printf("Speed increased!\n");
                        }
                    }
                    else if (activePowers[row][column] == 5) {
                        printf("Picked up power-up: +1 Bombs\n");
                        if (player->bombsAvailable < 5) {				// Max 5 bomber, (plockar up max 4 uppgraderingar)
                            player->bombsAvailable++;
                            printf("+1 Bombs!\n");
                        }
                    }
                    else if (activePowers[row][column] == 6) {
                        printf("Picked up power-up: Longer Explosion\n");
                        if (player->explosionRange < 5) {				// Max längd = mitten rutan + 5 rutor ut
                            player->explosionRange++;
                            printf("Explosion Range increased!\n");
                            packetData->explosionRange = player->explosionRange;
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
        //    printf("Bomb pos: x: %d\ty:%d\n", bombs[i]->bombPos.x, bombs[i]->bombPos.y);

            if (bombs[i]->spawnInside) {
                if (!(player->pos.x > right || left > player->pos.x + BOMB_SIZE || player->pos.y > down || player->pos.y + BOMB_SIZE < up))
                {
                    return true;
                }
                else
                {
                    bombs[i]->spawnInside = false;
                    return true;
                }
            }

            if (!(player->pos.x > right || left > player->pos.x + BOMB_SIZE || player->pos.y > down || player->pos.y + BOMB_SIZE < up))
            {
            //   printf("Char Pos: x: %d\ty: %d\n", p1->pos.x, p1->pos.y);
            //    printf("BOMB: Left: %d\tRight: %d\tUp: %d\tDown: %d\n ", left, right, up, down);
                return false;
            }
        }
    }
    return true;
}

/*
void removeBox(Player p1, Boxes boxes) {
    int indexRow = 0, indexCol = 0;
    int hitOne = 0, hitTwo = 0;
    int i = 0;
    int j = 0;
    bool passedGrey = false;
    bool illegalvalue = false;
    indexCol = ((((p1->pos.x + 32) / 64) * 64 + 7) / 64) - 1;
    indexRow = ((((p1->pos.y + 32) / 64) * 64 + 7) / 64) - 1;
    i = indexRow;
    j = indexCol;


    for (indexRow; i < indexRow + p1->explosionRange; i++)
    {

        if (boxes->activeBox[i][j] == 3)
        {
            passedGrey = true;
        }

        if (boxes->activeBox[i][j] == 1 && hitOne != p1->numOfBoxes && !passedGrey)
        {
            boxes->activeBox[i][j] = 0;
            hitOne++;

        }

        if (!illegalvalue) // ifall passerar 14
        {
            for (indexCol; j < indexCol + p1->explosionRange; j++)
            {

                if (boxes->activeBox[indexRow][j] == 3)
                {
                    break;
                }

                if (boxes->activeBox[indexRow][j] == 1 && hitTwo != p1->numOfBoxes)
                {
                    boxes->activeBox[indexRow][j] = 0;
                    hitTwo++;
                }
                if (j == 14)
                {
                    illegalvalue = true;
                    break;
                }

            }
        }
        j = indexCol;

        if (i == 10)
        {
            break;
        }
     

    } // kollar h�ger och ner�t 

    passedGrey = false;
    hitOne = 0;
    hitTwo = 0;
    i = indexRow;
    j = indexCol;
    illegalvalue = false;



    for (indexRow; i > indexRow - p1->explosionRange; i--)
    {
        if (boxes->activeBox[i][j] == 3)
        {
            passedGrey = true;
        }

        if (boxes->activeBox[i][j] == 1 && hitOne != p1->numOfBoxes && !passedGrey)
        {
            boxes->activeBox[i][j] = 0;
            hitOne++;

        }
        if (!illegalvalue) {
            for (indexCol; j > indexCol - p1->explosionRange; j--)
            {
                printf("active box value %d\n", boxes->activeBox[i][j]);
                if (boxes->activeBox[indexRow][j] == 3)
                {
                    break;
                }

                if (boxes->activeBox[indexRow][j] == 1 && hitTwo != p1->numOfBoxes)
                {
                    boxes->activeBox[indexRow][j] = 0;
                    hitTwo++;
                }
                if (j == 0)
                {
                    illegalvalue = true;
                    break;
                }

            }
        }
        j = indexCol;

        if (i == 0)
        {
            break;
        }

    } // kolla v�nster och upp�t

} */

void powerUpGone(int row, int col, int value) {
    activePowers[row][col] = value;
}



void move(Player player,int *lastMove, int *newMove, char key, Bomb bombs[], int *frames, Network net, udpData packetData) {
    if (player->prevPosX == 0)
    {
        player->prevPosX = player->pos.x;
        player->prevPosY = player->pos.y;
    }
    switch (key) {
    case 's':
        player->pos.y += player->speed;
        if (!checkCollision(player, bombs)) {
            player->pos.y -= player->speed;
        }
        if (*newMove == *lastMove && player->currentFrame <= 3) {
            *frames += 1;
            if (*frames == CHAR_REFRESH_RATE / player->speed) {
                *frames = 0;
                *lastMove = *newMove;
                player->currentFrame++;
                if (player->currentFrame > 3) {
                    player->currentFrame = 0;
                }
            }
        }
        else {
            *frames = 0;
            player->currentFrame = 0;
            *lastMove = *newMove = 0;
        }
        break;

    case 'w':
        player->pos.y -= player->speed;
        if (!checkCollision(player, bombs)) {
            player->pos.y += player->speed;
        }
        if (*newMove == *lastMove && player->currentFrame <= 7 && player->currentFrame > 3) {
            *frames += 1;
            if (*frames == CHAR_REFRESH_RATE / player->speed) {
                *frames = 0;
                *lastMove = *newMove;
                player->currentFrame++;
                if (player->currentFrame > 7) {
                    player->currentFrame = 4;
                }
            }
        }
        else {
            *frames = 0;
            player->currentFrame = 4;
            *lastMove = *newMove = 4;
        }
        break;

    case 'a':
        player->pos.x -= player->speed;
        if (!checkCollision(player, bombs)) {
            player->pos.x += player->speed;
        }
        if (*newMove == *lastMove && player->currentFrame <= 15 && player->currentFrame > 11) {
            *frames += 1;
            if (*frames == CHAR_REFRESH_RATE / player->speed) {
                *frames = 0;
                *lastMove = *newMove;
                player->currentFrame++;
                if (player->currentFrame > 15) {
                    player->currentFrame = 12;
                }
            }
        }
        else {
            *frames = 0;
            player->currentFrame = 12;
            *lastMove = *newMove = 12;
        }
        break;

    case 'd':
        player->pos.x += player->speed;
        if (!checkCollision(player, bombs)) {
            player->pos.x -= player->speed;
        }
        if (*newMove == *lastMove && player->currentFrame <= 11 && player->currentFrame > 7) {
            *frames += 1;
            if (*frames == CHAR_REFRESH_RATE / player->speed) {
                *frames = 0;
                *lastMove = *newMove;
                player->currentFrame++;
                if (player->currentFrame > 11) {
                    player->currentFrame = 8;
                }
            }
        }
        else {
            *frames = 0;
            player->currentFrame = 8;
            *lastMove = *newMove = 8;
        }
        break;

    default: break;
    }

    if (4 < abs(player->prevPosX - player->pos.x) || 4 < abs(player->prevPosY - player->pos.y))
    {
            packetData->xPos = player->pos.x;
            packetData->yPos = player->pos.y;
            packetData->frame = player->currentFrame;
            player->prevPosX = player->pos.x; 
            player->prevPosY = player->pos.y; 
            net->willSend = true;

    }
}