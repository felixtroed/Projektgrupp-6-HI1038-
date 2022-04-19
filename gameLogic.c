#include "gameLogic.h"

#define SCREENMAX_X 970
#define SCREENMIN_X 48

#define SCREENMAX_Y 700
#define SCREENMIN_Y 48


bool checkMovement(Player p1) {
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


bool checkIFBoxes()
{


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