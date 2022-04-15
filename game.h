#ifndef GAME_H
#define GAME_H

typedef struct GameSettings *Game;

#include <SDL2/SDL.h>
#include "player.h"

struct GameSettings {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *bitmapSurface;         //Anv�nds f�r bakgrunden
    SDL_Texture *background;             //Anv�nds f�r bakgrunden
    SDL_Event event;
    Player p1;
};

Game createGame();
void updateGame(Game game);
void exitGame(Game game);

#endif /* GAME_H */