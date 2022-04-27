#include "gameLogic.h"

#define SCREENMAX_X 970
#define SCREENMIN_X 48
#define SCREENMAX_Y 700
#define SCREENMIN_Y 48

#define BOMB_SIZE 34

Uint32 disableInvincibility(Uint32 interval, void *args);

void handleExplosionCollision(Game game) {
    if (game->p1->isHurt == false) {
        for (uint8_t i = 0; i < BOMBS; i++) {
            if (game->bombs[i] != NULL) {
                game->p1->hitboxPos.x = game->p1->pos.x + 16;       // Puts hitbox in the same position as character
                game->p1->hitboxPos.y = game->p1->pos.y + 14;

                if (SDL_HasIntersection(&game->p1->hitboxPos, &game->bombs[i]->explosionHor) || SDL_HasIntersection(&game->p1->hitboxPos, &game->bombs[i]->explosionVer)) {
                    game->p1->isHurt = true;
                    game->p1->invincibleTimer = SDL_AddTimer(2000, disableInvincibility, game->p1);
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
    int posBoxX=0, posBoxY=0;
     int sizeBox = 50;
     int sizePlayer = 50, sizePlayerGreyBox = 0;
     for (int row = 0; row < ROW_SIZE; row++) {
         for (int column = 0; column < COLUMN_SIZE; column++) {
             if (activeBox[row][column] >0)
             {
            
                 posBoxX = column * 64 + 64;
                 posBoxY = row * 64 + 64;
                 if (activeBox[row][column] == 2)
                 {
                     sizeBox = 32;
                     sizePlayer = 32;
                     sizePlayerGreyBox = 16;
                 }

                 if (!(p1->pos.x> posBoxX + sizeBox || posBoxX> p1->pos.x + sizePlayer  || p1->pos.y> posBoxY + sizeBox || 
                     p1->pos.y + sizePlayerGreyBox + sizePlayer < posBoxY))
                 {
                    //  printf("POS X: %d POS Y %d \n", p1->pos.x, p1->pos.y);
                    //  printf("BOX: POS X: %d POS Y:%d\n ", posBoxX, posBoxY);
                     return false; 
                 }  
             }
             sizeBox = 50;
             sizePlayer = 50;
             sizePlayerGreyBox = 0; 
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
        //        printf("Char Pos: x: %d\ty: %d\n", p1->pos.x, p1->pos.y);
        //        printf("BOMB: Left: %d\tRight: %d\tUp: %d\tDown: %d\n ", left, right, up, down);
                return false;
            }
        }
    }
    return true;
}


void move(Player p1,int *lastMove, int *newMove, char key, Bomb bombs[]) {
    switch (key) {
    case 's':
        p1->pos.y += p1->speed;
        if (!checkCollision(p1, bombs))
        {
            p1->pos.y -= p1->speed; 
        }

        if (*newMove == *lastMove && p1->currentFrame < 3)
        {
            p1->currentFrame++;
            *lastMove = *newMove;
            // printf("%d Down\n", p1->pos.y);
        }
        else
        {
            p1->currentFrame = 0;
            *lastMove = *newMove = 0;
            // printf("%d Down\n", p1->pos.y);
        }
        break;

    case 'w':
        p1->pos.y -= p1->speed;
        if (!checkCollision(p1, bombs))
        {
            p1->pos.y += p1->speed;
        }
        if (*newMove == *lastMove && p1->currentFrame < 7 && p1->currentFrame >3)
        {
            p1->currentFrame++;
            *lastMove = *newMove;
            // printf("%d Up\n", p1->pos.y);
        }
        else {
            p1->currentFrame = 4;
            *lastMove = *newMove = 4;
            // printf("%d Up\n", p1->pos.y);
        }
        break;

    case 'a':
        p1->pos.x -= p1->speed;
        if (!checkCollision(p1, bombs))
        {
            p1->pos.x += p1->speed;
        }
        
        if (*newMove == *lastMove && p1->currentFrame < 15 && p1->currentFrame >11)
        {
            p1->currentFrame++;
            *lastMove = *newMove;
            // printf("%d Left\n", p1->pos.x);
        }
        else {
            p1->currentFrame = 12;
            *lastMove = *newMove = 12;
            // printf("%d Left\n", p1->pos.x);
        }
        break;

    case 'd':
        p1->pos.x += p1->speed;
        if (!checkCollision(p1, bombs))
        {
            p1->pos.x -= p1->speed;
        }
        if (*newMove == *lastMove && p1->currentFrame < 11 && p1->currentFrame >7)
        {
            p1->currentFrame++;
            *lastMove = *newMove;
            // printf("%d Right\n", p1->pos.x);
        }
        else {
            p1->currentFrame = 8;
            *lastMove = *newMove = 8;
            // printf("%d Right\n", p1->pos.x);
        }
       break;

    default: break;
    }
}