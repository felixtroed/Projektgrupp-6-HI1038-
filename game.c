#include "game.h"
#include "box.h"
#include <stdio.h>
#include <stdbool.h>

#define PUBLIC /* empty */
#define PRIVATE static

#define WINDOW_WIDTH 1088
#define WINDOW_HEIGHT 832
#define ROW_SIZE 11
#define COLUMN_SIZE 15

struct GameSettings {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *bitmapSurface;         //Används för ladda upp bilder
    SDL_Texture *background;             //Används för att ladda in bilden av bakgrunden
    SDL_Texture *box;                   //Används för att ladda in bilden av lådan
    SDL_Rect position;                  //Används för positionen av lådorna
    SDL_Event event;
};

PUBLIC Game createGame() {
    Game game = malloc(sizeof(struct GameSettings));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL failed to initialize. Error code: %s\n", SDL_GetError());
        return NULL;
    }
    printf("SDL initialized!\n");

    game->window = SDL_CreateWindow("Exploder Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
    else {
        printf("Renderer created.\n");
    }
    
    game->bitmapSurface = SDL_LoadBMP("Walls/Background.bmp");                      //Laddar upp bakgrundsbilden till bitmapSurface (kanske måste ändra bildens position)
    if (!game->bitmapSurface) {
        printf("Could not load Background to bitmapSurface: %s\n", SDL_GetError());
    }
    else {
        printf("Background loaded to bitmapSurface.\n");
    }

    game->background = SDL_CreateTextureFromSurface(game->renderer, game->bitmapSurface);    //Skapar en Texture från bitmapSurface
    if (!game->background) {
        printf("Could not create texture from bitmapSurface: %s\n", SDL_GetError());
    }
    else {
        printf("Background loaded as texture.\n");
    }

    SDL_FreeSurface(game->bitmapSurface);                                           //Raderar bitmapSurface (frigör minnet) (Background finns fortfarande kvar)
    if (!game->background) {
        printf("Could not free bitmapSurface: %s\n", SDL_GetError());
    }
    else {
        printf("BitmapSurface freed.\n");
    }

    game->bitmapSurface = SDL_LoadBMP("Walls/Box.bmp");                      //Laddar upp bakgrundsbilden till bitmapSurface (kanske måste ändra bildens position)
    if (!game->bitmapSurface) {
        printf("Could not load Box to bitmapSurface: %s\n", SDL_GetError());
    }
    else {
        printf("Box loaded to bitmapSurface.\n");
    }

    game->box = SDL_CreateTextureFromSurface(game->renderer, game->bitmapSurface);    //Skapar en Texture från bitmapSurface
    if (!game->box) {
        printf("Could not create texture from bitmapSurface: %s\n", SDL_GetError());
    }
    else {
        printf("Box loaded as texture.\n");
    }
    
    SDL_FreeSurface(game->bitmapSurface);                                           //Raderar bitmapSurface (frigör minnet) (Background finns fortfarande kvar)
    if (!game->box) {
        printf("Could not free bitmapSurface: %s\n", SDL_GetError());
    }
    else {
        printf("BitmapSurface freed.\n");
    }
    
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
        SDL_RenderClear(game->renderer);
        SDL_RenderCopy(game->renderer, game->background, NULL, NULL);

        //// RENDERAR LÅDORNA, INTE OPTIMERAT ////
        game->position.w = 64;                  //Utanför loopen, alltid samma värde (höjd/bredd på lådan)
        game->position.h = 64;
        for (int row = 0; row < ROW_SIZE; row++) {
            for (int column = 0; column < COLUMN_SIZE; column++) {
                if (activeBox[row][column] != 0) {
                    game->position.x = column * 64 + 64;
                    game->position.y = row * 64 + 64;
                    SDL_RenderCopy(game->renderer, game->box, NULL, &game->position);       // Renderar en låda i taget
                }
            }
        }
        ////

        SDL_RenderPresent(game->renderer);
    }
}

PUBLIC void exitGame(Game game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}