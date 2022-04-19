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

#define KEYDOWN 's'
#define KEYUP 'w'
#define KEYRIGHT 'd'
#define KEYLEFT 'a'


PRIVATE bool initWinRen(Game game); 
PRIVATE bool createBackground(Game game); 
PRIVATE bool createBoxes(Game game); 
PRIVATE void renderBackground(Game game);

PUBLIC Game createGame() {
    Game game = malloc(sizeof(struct GameSettings));

    if (initWinRen(game)) {
        if(createBackground(game)) {
            if (createBoxes(game)) {
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) 
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                }
            }
        }
    }
    game->p1 = createPlayer(64, 64, game);

    return game;
}

PUBLIC void updateGame(Game game) {
    bool running = true;
    int newMove = 1, lastMove = 0;

    while (running) {
        while (SDL_PollEvent(&game->event) != 0) {
            if (game->event.type == SDL_QUIT) {
                running = false; // st�nger programmet ifall man trycker p� kryss 
            }
            else if (game->event.type == SDL_KEYDOWN) { // ifall en knapp �r netryckt 
                switch (game->event.key.keysym.sym) {
                case SDLK_w: 
                    if(checkMovement(game->p1))
                        movement(game->p1, &newMove, &lastMove, KEYUP);
                    break;

                case SDLK_s:
                    if (checkMovement(game->p1))
                    movement(game->p1, &newMove, &lastMove, KEYDOWN);
                    break;

                case SDLK_a:
                    if (checkMovement(game->p1))
                    movement(game->p1, &newMove, &lastMove, KEYLEFT);
                    break;

                case SDLK_d: 
                    if (checkMovement(game->p1))
                    movement(game->p1, &newMove, &lastMove, KEYRIGHT);
                    break;

                }
            }
        }
        SDL_RenderClear(game->renderer);
        SDL_RenderCopy(game->renderer, game->background, NULL, NULL);
        SDL_RenderCopyEx(game->renderer, game->p1->texture, &game->p1->clip[game->p1->currentFrame], &game->p1->pos, 0, NULL, SDL_FLIP_NONE);
        renderBackground(game);  
        SDL_RenderPresent(game->renderer);
    }
}

PRIVATE bool initWinRen(Game game) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL failed to initialize. Error code: %s\n", SDL_GetError());
        return false;
    }

    game->window = SDL_CreateWindow("Exploder Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!game->window) {
        printf("Window could not be created. Error code: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!game->renderer) {
        printf("Renderer could not be created: %s\n", SDL_GetError());
        SDL_DestroyWindow(game->window);
        SDL_Quit();
        return false;
    }

    return true; 
}

PRIVATE bool createBackground(Game game) {

    game->bitmapSurface = SDL_LoadBMP("resources/Background.bmp");                      //Laddar upp bakgrundsbilden till bitmapSurface (kanske m�ste �ndra bildens position)
    if (!game->bitmapSurface) {
        printf("Could not load Background to bitmapSurface: %s\n", SDL_GetError());
        return false;
    }
  
    game->background = SDL_CreateTextureFromSurface(game->renderer, game->bitmapSurface);    //Skapar en Texture fr�n bitmapSurface
    if (!game->background) {
        return false;
    }

    SDL_FreeSurface(game->bitmapSurface);                                           //Raderar bitmapSurface (frig�r minnet) (Background finns fortfarande kvar)
    if (!game->background) {
        printf("Could not free bitmapSurface: %s\n", SDL_GetError());
        return false; 
    }
    
    return true;
}

PRIVATE bool createBoxes(Game game) {

    game->bitmapSurface = SDL_LoadBMP("resources/Box.bmp");                      //Laddar upp bakgrundsbilden till bitmapSurface (kanske m�ste �ndra bildens position)
    if (!game->bitmapSurface) {
        printf("Could not load Box to bitmapSurface: %s\n", SDL_GetError());
        return false;
    }
   
    game->box = SDL_CreateTextureFromSurface(game->renderer, game->bitmapSurface);    //Skapar en Texture fr�n bitmapSurface
    if (!game->box) {
        printf("Could not create texture from bitmapSurface: %s\n", SDL_GetError());
        return false;
    }
  
    SDL_FreeSurface(game->bitmapSurface);                                           //Raderar bitmapSurface (frig�r minnet) (Background finns fortfarande kvar)
    if (!game->box) {
        printf("Could not free bitmapSurface: %s\n", SDL_GetError());
        return false;
    }
    return true; 
}

PUBLIC void exitGame(Game game) {
    IMG_Quit();
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

PRIVATE void renderBackground(Game game) {
     //// RENDERAR L�DORNA, INTE OPTIMERAT ////
     game->boxPos.w = 64;                  //Utanf�r loopen, alltid samma v�rde (h�jd/bredd p� l�dan)
     game->boxPos.h = 64;
     for (int row = 0; row < ROW_SIZE; row++) {
         for (int column = 0; column < COLUMN_SIZE; column++) {
             if (activeBox[row][column] != 0) {
                 game->boxPos.x = column * 64 + 64;
                 game->boxPos.y = row * 64 + 64;
                 SDL_RenderCopyEx(game->renderer, game->box, NULL, &game->boxPos, 0, NULL, SDL_FLIP_NONE);       // Renderar en l�da i taget
             }
         }
     }
 }


