#ifndef PLAYER_H
#define PLAYER_H

typedef struct Bomberman* Bomberman;

#include "game.h"
#include <SDL.h>
#include <SDL_image.h>

Player createPlayer(int x, int y, Game game);
int getPlayerPositionX(Bomberman player);
int getPlayerPositionY(Bomberman player);

#endif /* PLAYER_H */