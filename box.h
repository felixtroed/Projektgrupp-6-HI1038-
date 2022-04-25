#ifndef BOX_H
#define BOX_H

#define ROW_SIZE 11
#define COLUMN_SIZE 15
#include "game.h"
#include <stdlib.h>
#include <stdio.h>

//bool activeBox[11][15];

typedef struct GameSettings* Game;

typedef struct BoxInit {
	int activeBox[ROW_SIZE][COLUMN_SIZE];
} *Boxes;

Boxes setBoxes(Game game);



#endif /* BOX_H */