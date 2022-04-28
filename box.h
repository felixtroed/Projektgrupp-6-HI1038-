#ifndef BOX_H
#define BOX_H

#define ROW_SIZE 11
#define COLUMN_SIZE 15
#define W 3
#include "game.h"
#include <stdlib.h>
#include <stdio.h>

int activeBox[ROW_SIZE][COLUMN_SIZE];

typedef struct GameSettings* Game;

typedef struct BoxInit {
	int activeBox[ROW_SIZE][COLUMN_SIZE];
} *Boxes;

Boxes createBoxes(Game game);


#endif /* BOX_H */