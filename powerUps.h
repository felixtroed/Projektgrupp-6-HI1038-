#ifndef POWERUPS_H
#define POWERUPS_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
// #include <SDL.h>
// #include <SDL_image.h>
#include "box.h"

#define POWERUP_SIZE 50

int activePowers[ROW_SIZE][COLUMN_SIZE]; 

typedef struct GameSettings* Game;

typedef struct PowerUPS {
	SDL_Rect pos;
	SDL_Texture* speed;
	SDL_Texture* moreBombs;
	SDL_Texture* biggerExplosions; 
	int activePowers[ROW_SIZE][COLUMN_SIZE];
}*PowerUPS;


PowerUPS createPowers(Game game); 
void renderPowerUps(Game game);

#endif /* POWERUPS_H */