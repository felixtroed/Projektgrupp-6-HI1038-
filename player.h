#include <SDL2/SDL.h>

#ifndef PLAYER_H
#define PLAYER_H

typedef struct PlayerSettings *Player;

Player createPlayer(int x, int y, int frame);

#endif /* PLAYER_H */