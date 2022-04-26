#ifndef BOX_H
#define BOX_H

#include <stdbool.h>

#define INNER_WALLS 35
#define ROW_SIZE 11
#define COLUMN_SIZE 15
#define W 2			        // Walls, can't have boxes spawn on walls

int activeBox[11][15];

#endif /* BOX_H */