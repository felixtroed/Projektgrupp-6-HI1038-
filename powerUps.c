#include "powerUps.h"
int activePowers[ROW_SIZE][COLUMN_SIZE] = { 0 };

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

			if (activePowers[row][column] == 4) {
				game->power->pos.x= column * 64 + 64 + 7;
				game->power->pos.y = row * 64 + 64 + 7;
				SDL_RenderCopy(game->renderer, game->power->speed, NULL, &game->power->pos);
				// printf("'activeBox[row][column] == 4' - passed.\n");
			}
			if (activePowers[row][column] == 5) {
				game->power->pos.x = column * 64 + 64 + 7;
				game->power->pos.y = row * 64 + 64 + 7;
				SDL_RenderCopy(game->renderer, game->power->moreBombs, NULL, &game->power->pos);
				// printf("'activeBox[row][column] == 5' - passed.\n");
			}
			if (activePowers[row][column] == 6)
			{
				game->power->pos.x = column * 64 + 64 + 7;
				game->power->pos.y = row * 64 + 64 + 7;
				SDL_RenderCopy(game->renderer, game->power->biggerExplosions, NULL, &game->power->pos);
				// printf("'activeBox[row][column] == 6' - passed.\n");
			}
		}
	}
}
