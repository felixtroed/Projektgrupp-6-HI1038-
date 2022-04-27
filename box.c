#include "box.h"

#define PUBLIC /* EMPTY */
#define PRIVATE static


Boxes createBoxes(Game game) {
	Boxes boxes = malloc(sizeof(struct BoxInit));
	int playField[ROW_SIZE][COLUMN_SIZE] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, W, 1, W, 0, W, 1, W, 1, W, 1, W, 1, W, 0},
	{0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0},
	{0, W, 1, W, 0, W, 1, W, 1, W, 1, W, 1, W, 1},
	{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
	{0, W, 1, W, 1, W, 1, W, 0, W, 1, W, 1, W, 1},
	{0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1},
	{0, W, 1, W, 1, W, 1, W, 0, W, 1, W, 1, W, 1},
	{0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0},
	{0, W, 1, W, 1, W, 1, W, 1, W, 1, W, 1, W, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

	//int placeBox=0; 
	//srand(time(0));
	for (int row = 0; row < ROW_SIZE; row++) {
	//placeBox = rand()%+1;
		for (int column = 0; column < COLUMN_SIZE; column++) {
			boxes->activeBox[row][column] = playField[row][column];
		}
	}

	return boxes;

}

/*PUBLIC bool activeBox[ROW_SIZE][COLUMN_SIZE] = {
	{0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0},
	{0, W, 0, W, 1, W, 0, W, 0, W, 1, W, 1, W, 0},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
	{1, W, 0, W, 0, W, 1, W, 1, W, 1, W, 1, W, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1},
	{1, W, 1, W, 1, W, 1, W, 0, W, 1, W, 1, W, 1},
	{1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1},
	{1, W, 1, W, 1, W, 1, W, 0, W, 1, W, 1, W, 1},
	{0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0},
	{0, W, 1, W, 1, W, 1, W, 1, W, 1, W, 1, W, 0},
	{0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0}
}; */






