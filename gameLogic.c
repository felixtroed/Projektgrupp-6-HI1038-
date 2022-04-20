#include "gameLogic.h"

#define SCREENMAX_X 970
#define SCREENMIN_X 48
#define SCREENMAX_Y 700
#define SCREENMIN_Y 48

#define ROW_SIZE 11
#define COLUMN_SIZE 15 



bool checkCollision(Player p1,char key) {
    if (!collisionMap(p1))
        return false; 
    if (!collisionBoxes(p1, key))
        return false; 
 


    return true; 
}

bool collisionMap(Player p1) {
    if (p1->pos.y < SCREENMIN_Y)
    {
        p1->pos.y += p1->speed; 
        return false; 
    }

    if (p1->pos.y > SCREENMAX_Y)
    {
        p1->pos.y -= p1->speed;
        return false;
    }

    if (p1->pos.x < SCREENMIN_X)
    {
        p1->pos.x += p1->speed;
        return false;
    }


    if (p1->pos.x > SCREENMAX_X)
    {
        p1->pos.x -= p1->speed;
        return false;
    }

    return true;
}


bool collisionBoxes(Player p1,char key)
{
    int posBoxX, posBoxY;
     int sizeBox = 64; 
     int sizePlayer = 54;
        for (int row = 0; row < ROW_SIZE; row++) {
            sizeBox = 64; 
            for (int column = 0; column < COLUMN_SIZE; column++) {
                if (activeBox[row][column]>0)
                {
                    posBoxX = column * 64 + 64;
                    posBoxY= row * 64 + 64;

                    if (!(p1->pos.x > posBoxX + sizeBox || posBoxX > p1->pos.x + sizePlayer|| p1->pos.y > posBoxY + sizeBox
                        || p1->pos.y + sizePlayer < posBoxY)) {
                        switch (key) {
                        case 'd':  p1->pos.x -= p1->speed; p1->pos.y += p1->speed; return false;
                           
                        case 'a':  p1->pos.x += p1->speed;  p1->pos.y += p1->speed;  return false;
                          

                        case 's': p1->pos.y -= p1->speed;p1->pos.x += p1->speed; return false;
                            

                        case 'w': p1->pos.y += p1->speed;p1->pos.x += p1->speed; return false;

                        }

                    }
                }
            }
        }

    return true; 
}


void movement(Player p1,int *lastMove, int *newMove, char key) {
    switch (key) {
    case 's':
        p1->pos.y += p1->speed;
        if (*newMove == *lastMove && p1->currentFrame < 3)
        {
            p1->currentFrame++;
            *lastMove = *newMove;
            printf("%d Down\n", p1->pos.y);
        }
        else
        {
            p1->currentFrame = 0;
            *lastMove = *newMove = 0;
            printf("%d Down\n", p1->pos.y);
        }  // ändrar frame beronde på förregående flagga
        break;

    case 'w':
        p1->pos.y -= p1->speed;
        if (*newMove == *lastMove && p1->currentFrame < 7 && p1->currentFrame >3)
        {
            p1->currentFrame++;
            *lastMove = *newMove;
            printf("%d Up\n", p1->pos.y);

        }
        else {
            p1->currentFrame = 4;
            *lastMove = *newMove = 4;
            printf("%d Up\n", p1->pos.y);
        }
        break;

    case 'a':
        p1->pos.x -= p1->speed;
        if (*newMove == *lastMove && p1->currentFrame < 15 && p1->currentFrame >11)
        {
            p1->currentFrame++;
            *lastMove = *newMove;
            printf("%d Left\n", p1->pos.x);
        }
        else {
            p1->currentFrame = 12;
            *lastMove = *newMove = 12;
            printf("%d Left\n", p1->pos.x);

        }
        break;
    case 'd':

        p1->pos.x += p1->speed;
        if (*newMove == *lastMove && p1->currentFrame < 11 && p1->currentFrame >7)
        {
            p1->currentFrame++;
            *lastMove = *newMove;
            printf("%d Right\n", p1->pos.x);
        }
        else {
            p1->currentFrame = 8;
            *lastMove = *newMove = 8;
            printf("%d Right\n", p1->pos.x);

        }
        break;
    case SDLK_SPACE:
        printf("Space\n");
        break;
    default: break;
    }
}