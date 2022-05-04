#ifndef POWERUPS_H
#define POWERUPS_H


//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
#include <SDL.h>
#include <SDL_image.h>
#include "box.h"

#define POWERUP_SIZE 50

typedef struct GameSettings* Game;

typedef struct PowerUPS {
	SDL_Rect pos;
	SDL_Texture* speed;
	SDL_Texture* moreBombs;
	SDL_Texture* biggerExplosions; 
}*PowerUPS;


PowerUPS createPowers(Game game); 
void renderPowerUps(Game game);
void pickUpPowerUps(Player p1);

#endif /* POWERUPS_H */