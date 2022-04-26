#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H


#include "game.h"
#include "player.h"
#include "box.h"
#include <stdbool.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>


#define ROW_SIZE 11
#define COLUMN_SIZE 15
typedef struct BoxInit* Boxes;
typedef struct PlayerSettings* Player;
typedef struct GameSettings* Game;

bool collisionMap(Player p1);
void move(Player p1, int* lastMove, int* newMove, char key,Boxes boxes);
bool collisionBoxes(Player p1, Boxes boxes);
bool checkCollision(Player p1,Boxes boxes);
void removeBox(Player p1,Boxes boxes);

#endif /* GAME_LOGIC_H */