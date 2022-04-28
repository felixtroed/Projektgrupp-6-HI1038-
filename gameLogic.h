#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "game.h"
#include "player.h"
#include "box.h"
#include "bomb.h"
#include <stdbool.h>
// #include <SDL.h>
#include <math.h>
#include <SDL2/SDL.h>

typedef struct PlayerSettings* Player;
typedef struct GameSettings* Game;
typedef struct BombSettings* Bomb;
typedef struct BoxInit* Boxes;


bool collisionMap(Player p1);
void move(Player p1, int* lastMove, int* newMove, char key, Bomb bombs[], Boxes boxes, int *frames);
bool collisionBoxes(Player p1,Boxes boxes);
bool checkCollision(Player p1, Bomb bombs[], Boxes boxes);
bool collisionBomb(Player p1, Bomb bombs[]);
void handlePlayerExplosionCollision(Game game);
void removeBox(Player p1, Boxes boxes);

#endif /* GAME_LOGIC_H */