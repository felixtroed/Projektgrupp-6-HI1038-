#ifndef BOMB_H
#define BOMB_H

//#include <SDL2/SDL.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "player.h"
#include "box.h"

typedef struct GameSettings* Game;
typedef struct PlayerSettings* Player;
typedef struct BoxInit* Boxes;
typedef struct NetworkData* Network;
typedef struct udpData* udpData;

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
    int pIdxWhoDroppedBomb;
} *Bomb;

typedef struct BombTimerCallbackArgs {
    Bomb bomb;
    uint8_t *bombsAvailable;
    uint8_t pIdx;
    udpData packetData;
    Network net;
    int leftBoxVal;
    int leftBoxRow;
    int leftBoxCol;
} BombTimerCallbackArgs;

void bombPlacement(Player p, Bomb bombs[], uint8_t pIdx, SDL_Renderer *renderer, Network net, udpData packetData);
void initBombs(Bomb bombs[]);
Bomb createBomb(int playerPosX, int playerPosY, uint8_t pIdx, SDL_Renderer *renderer, int explosionRange);
void renderBombsAndExplosions(Game game, Network net, udpData packetData);
uint8_t getBombIdx(Bomb bombs[]);
Uint32 redBomb(Uint32 interval, void *switchToRedBomb);
Uint32 explodeBomb(Uint32 interval, void *args);
Uint32 explodeBombClient(Uint32 interval, void *args);
Uint32 explosionDoneClient(Uint32 interval, void *args);

#endif /* BOMB_H */