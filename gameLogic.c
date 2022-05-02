#include "gameLogic.h"

#define SCREENMAX_X 970
#define SCREENMIN_X 48
#define SCREENMAX_Y 700
#define SCREENMIN_Y 48

#define CHAR_REFRESH_RATE 20                // Higher value = Slower animation transition

#define BOMB_SIZE 34

Uint32 disableInvincibility(Uint32 interval, void *args);

void handlePlayerExplosionCollision(Game game) {
    if (game->player[game->pIdx]->isHurt == false) { 

        for (uint8_t i = 0; i < BOMBS; i++) {
            if (game->bombs[i] != NULL) {
                game->player[game->pIdx]->hitboxPos.x = game->player[game->pIdx]->pos.x + 16;       // Puts hitbox in the same position as character
                game->player[game->pIdx]->hitboxPos.y = game->player[game->pIdx]->pos.y + 14;

                if (game->bombs[i]->startExplosion == true && game->bombs[i]->endExplosion == false) {
                    if (SDL_HasIntersection(&game->player[game->pIdx]->hitboxPos, &game->bombs[i]->explosionHor) || SDL_HasIntersection(&game->player[game->pIdx]->hitboxPos, &game->bombs[i]->explosionVer)) {
                        game->player[game->pIdx]->isHurt = true;
                        
                        (game->player[game->pIdx]->lifes)--;
                        if (game->player[game->pIdx]->lifes <= 0) {
                            game->player[game->pIdx]->isAlive = false;
                            printf("Player died.\n");
                            return;
                        }

                        game->player[game->pIdx]->invincibleTimer = SDL_AddTimer(2000, disableInvincibility, game->player[game->pIdx]);
                    }
                }  
            } 
        }
    }
}

Uint32 disableInvincibility(Uint32 interval, void *args) {
    Player p = (Player) args;
    p->isHurt = false;
    return 0;
}

bool checkCollision(Player p1, Bomb bombs[]) {
    if (!collisionMap(p1))
        return false; 
    if (!collisionBoxes(p1))
        return false; 
    if (!collisionBomb(p1, bombs))
        return false;

    return true; 
}

bool collisionMap(Player p1) {
    if (p1->pos.y < SCREENMIN_Y)
    {
        return false; 
    }

    if (p1->pos.y > SCREENMAX_Y)
    {
        return false;
    }

    if (p1->pos.x < SCREENMIN_X)
    {
    
        return false;
    }

    if (p1->pos.x > SCREENMAX_X)
    {
        return false;
    }
    return true;
}

bool collisionBoxes(Player p1)
{
    int posBoxX = 0, posBoxY = 0;
    int sizeBox = 32;
    int sizePlayer = 32;
    const int botPlayerBigger = 16;

    for (int row = 0; row < ROW_SIZE; row++) {
        for (int column = 0; column < COLUMN_SIZE; column++) {
            if (activeBox[row][column] > 0)
            {

                posBoxX = column * 64 + 64;
                posBoxY = row * 64 + 64;

                if (!(p1->pos.x > posBoxX + sizeBox || posBoxX > p1->pos.x + sizePlayer || p1->pos.y > posBoxY + sizeBox ||
                    p1->pos.y + botPlayerBigger + sizePlayer < posBoxY))
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


bool collisionBomb(Player p1, Bomb bombs[]) {
    for (uint8_t i = 0; i < BOMBS; i++) {
        if (bombs[i] != NULL) {
            int left = (bombs[i]->bombPos.x);
            int right = (bombs[i]->bombPos.x) + ((64 - BOMB_SIZE) / 2 - 7);
            int up = (bombs[i]->bombPos.y);
            int down = (bombs[i]->bombPos.y) + ((64 - BOMB_SIZE) / 2 - 7);
        //    printf("Bomb pos: x: %d\ty:%d\n", bombs[i]->bombPos.x, bombs[i]->bombPos.y);

            if (bombs[i]->spawnInside) {
                if (!(p1->pos.x > right || left > p1->pos.x + BOMB_SIZE || p1->pos.y > down || p1->pos.y + BOMB_SIZE < up))
                {
                    return true;
                }
                else
                {
                    bombs[i]->spawnInside = false;
                    return true;
                }
            }

            if (!(p1->pos.x > right || left > p1->pos.x + BOMB_SIZE || p1->pos.y > down || p1->pos.y + BOMB_SIZE < up))
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


void move(Player p1,int *lastMove, int *newMove, char key, Bomb bombs[], int *frames, Network net, uint8_t pIdx) {
    int prevXPos = p1->pos.x;
    int prevYPos = p1->pos.y;
    
    switch (key) {
    case 's':
        p1->pos.y += p1->speed;
        if (!checkCollision(p1, bombs)) {
            p1->pos.y -= p1->speed;
        }
        if (*newMove == *lastMove && p1->currentFrame <= 3) {
            *frames += 1;
            if (*frames == CHAR_REFRESH_RATE / p1->speed) {
                *frames = 0;
                *lastMove = *newMove;
                p1->currentFrame++;
                if (p1->currentFrame > 3) {
                    p1->currentFrame = 0;
                }
            }
        }
        else {
            *frames = 0;
            p1->currentFrame = 0;
            *lastMove = *newMove = 0;
        }
        break;

    case 'w':
        p1->pos.y -= p1->speed;
        if (!checkCollision(p1, bombs)) {
            p1->pos.y += p1->speed;
        }
        if (*newMove == *lastMove && p1->currentFrame <= 7 && p1->currentFrame > 3) {
            *frames += 1;
            if (*frames == CHAR_REFRESH_RATE / p1->speed) {
                *frames = 0;
                *lastMove = *newMove;
                p1->currentFrame++;
                if (p1->currentFrame > 7) {
                    p1->currentFrame = 4;
                }
            }
        }
        else {
            *frames = 0;
            p1->currentFrame = 4;
            *lastMove = *newMove = 4;
        }
        break;

    case 'a':
        p1->pos.x -= p1->speed;
        if (!checkCollision(p1, bombs)) {
            p1->pos.x += p1->speed;
        }
        if (*newMove == *lastMove && p1->currentFrame <= 15 && p1->currentFrame > 11) {
            *frames += 1;
            if (*frames == CHAR_REFRESH_RATE / p1->speed) {
                *frames = 0;
                *lastMove = *newMove;
                p1->currentFrame++;
                if (p1->currentFrame > 15) {
                    p1->currentFrame = 12;
                }
            }
        }
        else {
            *frames = 0;
            p1->currentFrame = 12;
            *lastMove = *newMove = 12;
        }
        break;

    case 'd':
        p1->pos.x += p1->speed;
        if (!checkCollision(p1, bombs)) {
            p1->pos.x -= p1->speed;
        }
        if (*newMove == *lastMove && p1->currentFrame <= 11 && p1->currentFrame > 7) {
            *frames += 1;
            if (*frames == CHAR_REFRESH_RATE / p1->speed) {
                *frames = 0;
                *lastMove = *newMove;
                p1->currentFrame++;
                if (p1->currentFrame > 11) {
                    p1->currentFrame = 8;
                }
            }
        }
        else {
            *frames = 0;
            p1->currentFrame = 8;
            *lastMove = *newMove = 8;
        }
        break;

    default: break;
    }

    // Send position
    if(prevXPos != p1->pos.x || prevYPos != p1->pos.y) {
        // printf("%d %d\n", p1->pos.x, p1->pos.y);
        sprintf((char *)net->packet1->data, "%d %d %d %d\n", pIdx, p1->pos.x, p1->pos.y, p1->currentFrame);    
        net->packet1->address.host = net->srvAddr.host;	                    /* Set the destination host */
        net->packet1->address.port = net->srvAddr.port;	                    /* And destination port */
        net->packet1->len = strlen((char *)net->packet1->data) + 1;
        SDLNet_UDP_Send(net->sd, -1, net->packet1);
        prevXPos = p1->pos.x;
        prevYPos = p1->pos.y;
    }
}