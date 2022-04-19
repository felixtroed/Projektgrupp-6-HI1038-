#ifndef BOMB_H
#define BOMB_H

#include <SDL2/SDL.h>
// #include <SDL.h>
#include <stdbool.h>
#include "game.h"

typedef struct GameSettings* Game;

typedef struct Bomb {
    SDL_Texture *texture;
    SDL_Rect clip[2];
    SDL_Rect pos;
    SDL_Surface *surface;
    SDL_TimerID timer;
    int currentFrame;
    bool hasCollision;
    int explosionRange;
} *Bomb;

Bomb createBomb(int x, int y, Game game);

#endif /* BOMB_H */