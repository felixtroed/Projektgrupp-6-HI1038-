#include "game.h"
#include <stdio.h>
#include <stdbool.h>

#define PUBLIC /* empty */
#define PRIVATE static

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

struct GameSettings {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Event event;
};

PUBLIC Game createGame() {
    Game game = malloc(sizeof(struct GameSettings));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL failed to initialize. Error code: %s\n", SDL_GetError());
        return NULL;
    }
    printf("SDL initialized!\n");

    game->window = SDL_CreateWindow("Exploder man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!game->window) {
        printf("Window could not be created. Error code: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL; 
    }
    printf("Window created.\n");

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!game->renderer) {
        printf("Renderer could not be created: %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        return NULL;
    }
    printf("Renderer created.\n");

    return game;
}

PUBLIC void updateGame(Game game) {
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&game->event) != 0) {
            switch(game->event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                default: break;
            }
        }
    }
}

PUBLIC void exitGame(Game game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}