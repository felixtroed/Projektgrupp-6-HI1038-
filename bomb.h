#ifndef BOMB_H
#define BOMB_H

#include <SDL2/SDL.h>
// #include <SDL.h>
#include <stdbool.h>
#include "game.h"
#include "player.h"

typedef struct GameSettings* Game;

typedef struct Bomb {
    SDL_Texture *textureBomb;
    SDL_Texture *textureBombRed;
    SDL_Rect clip[2];
    SDL_Rect pos;
    SDL_Surface *surface;
    SDL_TimerID explosionTime;
    SDL_TimerID redBombTime;
    int currentFrame;
    bool exploded;
    bool switchRedBomb;
    bool hasCollision;
    int explosionRange;
} *Bomb;

void placeBomb(Game game);
void initBombs(Bomb bombs[]);
void renderBombs(Game game);

#endif /* BOMB_H */