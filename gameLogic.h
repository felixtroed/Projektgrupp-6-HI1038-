#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "game.h"
#include "player.h"
#include "box.h"
#include <stdbool.h>
// #include <SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.h"

typedef struct PlayerSettings* Player;
typedef struct GameSettings* Game;
bool collisionMap(Player p1);
void move(Player p1, int* lastMove, int* newMove, char key);
bool collisionBoxes(Player p1);
bool checkCollision(Player p1);

#endif /* GAME_LOGIC_H */