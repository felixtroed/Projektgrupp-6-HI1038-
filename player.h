#ifndef PLAYER_H
#define PLAYER_H

typedef struct PlayerSettings *Player;

#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct PlayerSettings {
    SDL_Rect position;
    SDL_Rect spriteClips[16];
    SDL_Texture *texture;
    SDL_Surface *surface;
    int frame;
};

Player createPlayer(int x, int y, int frame, Game game);

#endif /* PLAYER_H */