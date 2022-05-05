#include "powerUps.h"


powerMap[ROW_SIZE][COLUMN_SIZE] = {
	{0, 0, 4, 0, 0, 6, 0, 4, 0, 0, 6, 0, 0, 0, 0},
	{0, W, 1, W, 1, W, 0, W, 0, W, 4, W, 0, W, 0},
	{0, 0, 4, 0, 0, 0, 0, 0, 0, 6, 0, 5, 0, 0, 0},
	{4, W, 0, W, 0, W, 0, W, 5, W, 6, W, 0, W, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
	{6, W, 0, W, 4, W, 0, W, 0, W, 0, W, 0, W, 1},
	{4, 0, 0, 0, 6, 0, 5, 0, 0, 0, 4, 0, 0, 0, 5},
	{0, W, 0, W, 0, W, 0, W, 0, W, 0, W, 0, W, 0},
	{0, 0, 0, 0, 4, 0, 0, 0, 0, 5, 0, 0, 0, 6, 0},
	{0, W, 4, W, 0, W, 0, W, 0, W, 0, W, 0, W, 0},
	{0, 0, 0, 6, 0, 5, 0, 0, 4, 0, 6, 4, 0, 0, 0}
};


PowerUPS createPowers(Game game)
{
	PowerUPS power = malloc(sizeof(struct PowerUPS));

	power->pos.h = POWERUP_SIZE;
	power->pos.w = POWERUP_SIZE;

	char pictureDestination[64];

	SDL_strlcpy(pictureDestination, "resources/powUpSpeed.png", sizeof pictureDestination);
	loadTextures(&game->renderer, &game->bitmapSurface, &power->speed, pictureDestination);

	SDL_strlcpy(pictureDestination, "resources/powUpMorebombs.png", sizeof pictureDestination);
	loadTextures(&game->renderer, &game->bitmapSurface, &power->moreBombs, pictureDestination);

	SDL_strlcpy(pictureDestination, "resources/powUpExp.png", sizeof pictureDestination);
	loadTextures(&game->renderer, &game->bitmapSurface, &power->biggerExplosions, pictureDestination);

	return power;
}


void renderPowerUps(Game game) {

	for (int row = 0; row < ROW_SIZE; row++) {
		for (int column = 0; column < COLUMN_SIZE; column++) {
			printf("%d", powerMap[row][column]);
			if (powerMap[row][column] == 4) {
				game->power->pos.x= column * 64 + 64 + 7;
				game->power->pos.y = row * 64 + 64 + 7;
				SDL_RenderCopy(game->renderer, game->power->speed, NULL, &game->power->pos);   
			}
			if (powerMap[row][column] == 5) {
				game->power->pos.x = column * 64 + 64 + 7;
				game->power->pos.y = row * 64 + 64 + 7;
				SDL_RenderCopy(game->renderer, game->power->moreBombs, NULL, &game->power->pos);
			}
			if (powerMap[row][column] == 6)
			{
				game->power->pos.x = column * 64 + 64 + 7;
				game->power->pos.y = row * 64 + 64 + 7;
				SDL_RenderCopy(game->renderer, game->power->biggerExplosions, NULL, &game->power->pos);
			}
		}
	}
}
