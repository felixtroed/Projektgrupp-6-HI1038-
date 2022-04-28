#ifndef BOMB_H
#define BOMB_H

#include <SDL2/SDL.h>
// #include <SDL.h>
#include <stdbool.h>
#include "game.h"
#include "player.h"
#include "box.h"

typedef struct GameSettings* Game;
typedef struct PlayerSettings* Player;
typedef struct BoxInit* Boxes;

typedef struct BombSettings {
    SDL_Texture *textureBomb;
    SDL_Texture *textureBombRed;
    SDL_Texture* textureExplosionStart;
    SDL_Texture* textureExplosionMiddle;
    SDL_Texture* textureExplosionEnd;
    SDL_Rect bombPos;
    SDL_Rect explosionPos;
    SDL_Rect explosionHor;
    SDL_Rect explosionVer;
    SDL_Surface *surface;
    SDL_TimerID bombTime;
    SDL_TimerID redBombTime;
    SDL_TimerID deleteBombTime;
    bool startExplosion;
    bool endExplosion;
    bool switchRedBomb;
    bool spawnInside;
    int explosionRange;
    int boxIdxesLeft[2];
    int boxIdxesRight[2];
    int boxIdxesTop[2];
    int boxIdxesBottom[2];
} *Bomb;

void bombPlacement(Player p, Bomb bombs[], SDL_Renderer *renderer);
void initBombs(Bomb bombs[]);
void renderBombsAndExplosions(Game game);

#endif /* BOMB_H */