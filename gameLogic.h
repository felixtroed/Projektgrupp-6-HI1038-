#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "game.h"
#include "player.h"
#include "box.h"
#include <stdbool.h>
// #include <SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct PlayerSettings* Player;
typedef struct GameSettings* Game;

bool checkMovement(Player p1);
void movement(Player p1, int* lastMove, int* newMove, char key);
bool checkIfBoxes();

#endif /* GAME_LOGIC_H */