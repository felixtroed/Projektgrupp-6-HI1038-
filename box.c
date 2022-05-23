#include "box.h"

#define PUBLIC /* EMPTY */
#define PRIVATE static

Boxes createBoxes(Game game) {

		Boxes boxes = malloc(sizeof(struct BoxInit));
		char boxDestination[64] = "resources/Box.png";
		loadTextures(&game->renderer, &game->bitmapSurface, &boxes->box, boxDestination);
		return boxes;

} 



void renderBoxes(Game game) {
	game->boxPos.w = 64;                  
	game->boxPos.h = 64;
	for (int row = 0; row < ROW_SIZE; row++) {
		for (int column = 0; column < COLUMN_SIZE; column++) {
			if (activeBox[row][column] == 1) {
				game->boxPos.x = column * 64 + 64;
				game->boxPos.y = row * 64 + 64;
				SDL_RenderCopyEx(game->renderer, game->boxes->box, NULL, &game->boxPos, 0, NULL, SDL_FLIP_NONE);       // Render one box at the time.
			}
		}
	}
}

PUBLIC int activeBox[ROW_SIZE][COLUMN_SIZE] = {
	{0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0},
	{0, W, 1, W, 1, W, 0, W, 0, W, 1, W, 1, W, 0},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
	{1, W, 0, W, 0, W, 1, W, 1, W, 1, W, 1, W, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1},
	{1, W, 1, W, 1, W, 1, W, 0, W, 1, W, 1, W, 1},
	{1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1},
	{1, W, 1, W, 1, W, 1, W, 0, W, 1, W, 1, W, 1},
	{0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0},
	{0, W, 1, W, 1, W, 1, W, 1, W, 1, W, 1, W, 0},
	{0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0}
};

PUBLIC int resetBoxPos[ROW_SIZE][COLUMN_SIZE] = {
	{0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0},
	{0, W, 1, W, 1, W, 0, W, 0, W, 1, W, 1, W, 0},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
	{1, W, 0, W, 0, W, 1, W, 1, W, 1, W, 1, W, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1},
	{1, W, 1, W, 1, W, 1, W, 0, W, 1, W, 1, W, 1},
	{1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1},
	{1, W, 1, W, 1, W, 1, W, 0, W, 1, W, 1, W, 1},
	{0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0},
	{0, W, 1, W, 1, W, 1, W, 1, W, 1, W, 1, W, 0},
	{0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0}
};






