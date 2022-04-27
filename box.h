#ifndef BOX_H
#define BOX_H

#define ROW_SIZE 11
#define COLUMN_SIZE 15
#define W 3			// Walls, can't have boxes spawn on walls

#include <stdbool.h>

int activeBox[ROW_SIZE][COLUMN_SIZE];

#endif /* BOX_H */