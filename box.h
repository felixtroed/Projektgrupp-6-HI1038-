#ifndef BOX_H
#define BOX_H

#define ROW_SIZE 11
#define COLUMN_SIZE 15
#define W 3

#include "game.h"
#include <SDL.h>
//#include <SDL2/SDL.h>
int resetBoxPos[ROW_SIZE][COLUMN_SIZE];

typedef struct GameSettings* Game;
typedef struct BoxInit *Boxes;

Boxes createBoxes(Game game);
void renderBoxes(Game game);
int getActiveBox(Boxes boxes,int row,int col);
void setBoxValue(Boxes boxes, int row, int col);
void resetBoxes(Boxes boxes);


#endif /* BOX_H */