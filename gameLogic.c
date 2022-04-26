#include "gameLogic.h"

#define SCREENMAX_X 970
#define SCREENMIN_X 48
#define SCREENMAX_Y 700
#define SCREENMIN_Y 48

#define ROW_SIZE 11
#define COLUMN_SIZE 15 


bool checkCollision(Player p1,Boxes boxes) {
    if (!collisionMap(p1))
        return false; 
    if (!collisionBoxes(p1,boxes))
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


bool collisionBoxes(Player p1,Boxes boxes)
{
     int posBoxX=0, posBoxY=0;
     int sizeBox = 32;
     int sizePlayer = 32;
     const int botPlayerBigger = 16;

     for (int row = 0; row < ROW_SIZE; row++) {
         for (int column = 0; column < COLUMN_SIZE; column++) {
             if (boxes->activeBox[row][column] >0)
             {
            
                 posBoxX = column * 64 + 64;
                 posBoxY = row * 64 + 64;

                 if (!(p1->pos.x > posBoxX + sizeBox || posBoxX  > p1->pos.x + sizePlayer  || p1->pos.y > posBoxY + sizeBox || 
                     p1->pos.y + botPlayerBigger + sizePlayer < posBoxY))
                 {
                     printf("POS X: %d POS Y %d \n", p1->pos.x, p1->pos.y);
                     printf("BOX: POS X: %d POS Y:%d\n ", posBoxX, posBoxY);
                     return false; 

                 }  

             }
           
           
         }
     }
        return true;
}


void removeBox(Player p1,Boxes boxes) {

    int posBoxX = 0, posBoxY = 0;
    float closest=1000, distance;
    int indexOne, indexTwo;
    bool boxeGone = false;
 
    for (int row = 0; row < ROW_SIZE; row++) {
        for (int column = 0; column < COLUMN_SIZE; column++) {
            if (boxes->activeBox[row][column] == 1)
            {
              
                posBoxX = column * 64 + 64;
                posBoxY = row * 64 + 64;
                distance = sqrt(pow(posBoxX - p1->pos.x, 2) + pow((posBoxY - p1->pos.y), 2));
            
                if ( distance <closest && distance < 150 )
                {
                        indexOne = row; 
                        indexTwo = column;
                        printf("Row %d, Column %d\n", row, column);
                        closest = sqrt(pow(posBoxX - p1->pos.x, 2) + pow((posBoxY - p1->pos.y), 2));
                        printf("Player pos X %d Y %d \n", p1->pos.x, p1->pos.y);
                        printf("BOX pos X %d Y %d\n", posBoxX, posBoxY);
                        printf("Closest: %f\n", closest);
                        boxeGone = true;
             
                }
            }
      
        }
    }
    if(boxeGone)
    boxes->activeBox[indexOne][indexTwo] = 0;

}



void move(Player p1,int *lastMove, int *newMove, char key, Boxes boxes) {
    switch (key) {
    case 's':
        p1->pos.y += p1->speed;
        if (!checkCollision(p1,boxes))
        {
            p1->pos.y -= p1->speed; 
        }

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
        }


        break;

    case 'w':
        p1->pos.y -= p1->speed;
        if (!checkCollision(p1,boxes))
        {
            p1->pos.y += p1->speed;
        }
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
        if (!checkCollision(p1,boxes))
        {
            p1->pos.x += p1->speed;
        }
        
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
        if (!checkCollision(p1,boxes))
        {
            p1->pos.x -= p1->speed;
        }
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

