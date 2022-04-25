#ifndef BOMB_H
#define BOMB_H

//#include <SDL2/SDL.h>
#include <SDL.h>
#include <stdbool.h>
#include "game.h"
#include "player.h"

typedef struct GameSettings* Game;
typedef struct PlayerSettings* Player;

typedef struct Bomb {
    SDL_Texture *textureBomb;
    SDL_Texture *textureBombRed;
    SDL_Texture* textureExplosionStart;
    SDL_Texture* textureExplosionMiddle;
    SDL_Texture* textureExplosionEnd;
    SDL_Rect bombPos;
    SDL_Rect explosionPos;
    SDL_Surface *surface;
    SDL_TimerID bombTime;
    SDL_TimerID redBombTime;
    SDL_TimerID deleteBombTime;
    int currentFrame;
    bool startExplosion;
    bool endExplosion;
    bool switchRedBomb;
    bool hasCollision;
    int explosionRange;
} *Bomb;

void bombPlacement(Player p, Bomb bombs[], SDL_Renderer *renderer);
void initBombs(Bomb bombs[]);
void renderBombsAndExplosions(Game game);

#endif /* BOMB_H */