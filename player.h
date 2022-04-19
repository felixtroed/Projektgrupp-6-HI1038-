#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include <SDL.h>
#include <SDL_image.h>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>

typedef struct GameSettings* Game;

typedef struct PlayerSettings {
    SDL_Texture *texture;
    SDL_Rect clip[16];
    SDL_Rect pos;
    SDL_RendererFlip flip;
    SDL_Surface *surface;
    int currentFrame;
    int speed;
} *Player;

Player createPlayer(int x, int y, Game game);

// int getPlayerPositionX(Player player);
// int getPlayerPositionY(Player player);

#endif /* PLAYER_H */