#include "powerUps.h"

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
			if (activeBox[row][column] == 4) {
				game->power->pos.x= column * 64 + 64 + 7;
				game->power->pos.y = row * 64 + 64 + 7;
				SDL_RenderCopy(game->renderer, game->power->speed, NULL, &game->power->pos);   
			}
			if (activeBox[row][column] == 5) {
				game->power->pos.x = column * 64 + 64 + 7;
				game->power->pos.y = row * 64 + 64 + 7;
				SDL_RenderCopy(game->renderer, game->power->moreBombs, NULL, &game->power->pos);
			}
			if (activeBox[row][column] == 6)
			{
				game->power->pos.x = column * 64 + 64 + 7;
				game->power->pos.y = row * 64 + 64 + 7;
				SDL_RenderCopy(game->renderer, game->power->biggerExplosions, NULL, &game->power->pos);
			}
		}
	}
}


void pickUpPowerUps(Player p1) {
	//printf("char posX: %d\tposY: %d\n", p1->pos.x, p1->pos.y);
	for (int row = 0; row < ROW_SIZE; row++) {
		for (int column = 0; column < COLUMN_SIZE; column++) {
			if (activeBox[row][column] >= 4) {
				int powerUpLeft = column * 64 + 64 - 30;
				int powerUpRight = powerUpLeft + 64;
				int powerUpUp = row * 64 + 14;
				int powerUpDown = powerUpUp + 70;

				if (p1->pos.x > powerUpLeft && p1->pos.x < powerUpRight && p1->pos.y > powerUpUp && p1->pos.y < powerUpDown)
				{
					if (activeBox[row][column] == 4) {
						printf("Picked up power-up: Speed\n");
						if (p1->speed < 5) {				// Speed värdet startar 2, max 3 uppgraderingar
							p1->speed += 1;
							printf("Speed increased!\n");
						}
					}
					else if (activeBox[row][column] == 5) {
						printf("Picked up power-up: +1 Bombs\n");
						if (p1->bombsAvailable < 5) {				// Max 5 bomber, (plockar up max 4 uppgraderingar)
							p1->bombsAvailable += 1;
							printf("+1 Bombs!\n");
						}
					}
					else if (activeBox[row][column] == 6) {
						printf("Picked up power-up: Longer Explosion\n");
						if (p1->explosionRange < 5) {				// Max längd = mitten rutan + 5 rutor ut
							p1->explosionRange += 1;
							printf("Explosion Range increased!\n");
						}
					}
					activeBox[row][column] = 0;
				}
			}
		}
	}
}