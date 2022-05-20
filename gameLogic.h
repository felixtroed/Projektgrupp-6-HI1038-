#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "game.h"
#include "player.h"
#include "box.h"
#include "bomb.h"
#include <stdbool.h>
#include <SDL.h>
#include <math.h>
//#include <SDL2/SDL.h>

typedef struct PlayerSettings* Player;
typedef struct GameSettings* Game;
typedef struct BombSettings* Bomb;
typedef struct BoxInit* Boxes;
typedef struct NetworkData* Network;
typedef struct udpData* udpData;

bool collisionMap(Player player);
void move(Player player, int* lastMove, int* newMove, char key, Bomb bombs[], int *frames, Network net, udpData packetData);
bool collisionBoxes(Player player);
bool checkCollision(Player player, Bomb bombs[]);
bool collisionBomb(Player player, Bomb bombs[]);
void handlePlayerExplosionCollision(Game game, Network net, udpData packetData);
void powerUpGone(int row,int col,int value); 

// void removeBox(Player p1, Boxes boxes);

#endif /* GAME_LOGIC_H */