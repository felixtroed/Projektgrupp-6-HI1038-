#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>

typedef struct GameSettings* Game;

typedef struct PlayerSettings {
    SDL_Texture *texture;
    SDL_Rect clip[16];
    SDL_Rect pos;
    SDL_Rect hitboxPos;
    SDL_RendererFlip flip;
    SDL_Surface *surface;
    SDL_TimerID invincibleTimer;
    bool isHurt;
    int currentFrame;
    int speed;
    uint8_t bombsAvailable;
} *Player;

Player createPlayer(int playerNumber, int x, int y, Game game);

#endif /* PLAYER_H */