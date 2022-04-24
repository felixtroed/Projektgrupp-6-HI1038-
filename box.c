#include "box.h"

#define PUBLIC /* EMPTY */
#define PRIVATE static

#define ROW_SIZE 11
#define COLUMN_SIZE 15
#define W 3			// Walls, can't have boxes spawn on walls

PUBLIC int activeBox[ROW_SIZE][COLUMN_SIZE] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, W, 1, W, 0, W, 1, W, 1, W, 1, W, 1, W, 0},
	{0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0},
	{0, W, 1, W, 0, W, 1, W, 1, W, 1, W, 1, W, 1},
	{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
	{0, W, 1, W, 1, W, 1, W, 0, W, 1, W, 1, W, 1},
	{0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1},
	{0, W, 1, W, 1, W, 1, W, 0, W, 1, W, 1, W, 1},
	{0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0},
	{0, W, 1, W, 1, W, 1, W, 1, W, 1, W, 1, W, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
