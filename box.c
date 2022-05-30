#include "box.h"

#define PUBLIC /* EMPTY */
#define PRIVATE static

typedef struct BoxInit {
	int activeBox[ROW_SIZE][COLUMN_SIZE];
	SDL_Texture* box;                   
	SDL_Rect boxPos;
} *Boxes;

PUBLIC Boxes createBoxes(Game game) {
		Boxes boxes = malloc(sizeof(struct BoxInit));
		char boxDestination[64] = "resources/Box.png";
		loadTextures(&game->renderer, &game->bitmapSurface, &boxes->box, boxDestination);
		
		int activeBoxes[ROW_SIZE][COLUMN_SIZE] = {
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
		{0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0}};

		for (Uint8 row = 0;row < ROW_SIZE;row++)
		{
			for (Uint8 col = 0; col < COLUMN_SIZE;col++)
			{
				boxes->activeBox[row][col] = activeBoxes[row][col];

			}
		}
		return boxes;

} 


PUBLIC void renderBoxes(Game game) {
	game->boxes->boxPos.w = 64;                  
	game->boxes->boxPos.h = 64;
	for (int row = 0; row < ROW_SIZE; row++) {
		for (int column = 0; column < COLUMN_SIZE; column++) {
			if (game->boxes->activeBox[row][column] == 1) {
				game->boxes->boxPos.x = column * 64 + 64;
				game->boxes->boxPos.y = row * 64 + 64;
				SDL_RenderCopyEx(game->renderer, game->boxes->box, NULL, &game->boxes->boxPos, 0, NULL, SDL_FLIP_NONE);       // Render one box at the time.
			}
		}
	}
}

PUBLIC int getActiveBox(Boxes boxes,int row, int col) {
		return boxes->activeBox[row][col];
}

PUBLIC SDL_Rect* getBoxHitPoxPos(Boxes box)
{
	return &box->boxPos; 
}

PUBLIC void setBoxValue(Boxes boxes, int row, int col)
{
	 boxes->activeBox[row][col]=0;
}

PUBLIC void resetBoxes(Boxes boxes) {

	int resetBoxPos[ROW_SIZE][COLUMN_SIZE] = {
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

	for (Uint8 row = 0;row < ROW_SIZE;row++)
	{
		for (Uint8 col = 0; col < COLUMN_SIZE;col++)
		{
			boxes->activeBox[row][col] = resetBoxPos[row][col];

		}
	}
}







