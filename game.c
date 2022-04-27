#include "game.h"
#include <stdio.h>
#include <stdbool.h>

#define PUBLIC /* empty */
#define PRIVATE static

#define KEYDOWN 's'
#define KEYUP 'w'
#define KEYRIGHT 'd'
#define KEYLEFT 'a'

PRIVATE bool initWinRen(Game game);
PRIVATE bool createBackground(Game game);
PRIVATE bool createBoxes(Game game);
PRIVATE void renderBoxes(Game game);

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
    game->p1 = createPlayer(1, 64, 64, game);
    game->p2 = createPlayer(2, 960, 64, game);
    game->p3 = createPlayer(3, 64, 704, game);
    game->p4 = createPlayer(4, 960, 704, game);
    initBombs(game->bombs);                           // Sets all bombs to NULL

    return game;
}

PUBLIC void updateGame(Game game) {
    bool running = true;
    char keyresseed; 
    int newMove = 1, lastMove = 0;

    while (running) {
        bool keyUp=true;
        while (SDL_PollEvent(&game->event) != 0) 
        {
            switch (game->event.type)
            {
              case SDL_QUIT: running = false; 
                break; 
              case SDL_KEYUP: keyUp = true; 
                break; 
              case SDL_KEYDOWN: keyUp = false;
                  break; 

            }
            if (!keyUp) { // ifall en knapp �r netryckt

                switch (game->event.key.keysym.sym)
                {
                case SDLK_w:
                        move(game->p1, &newMove, &lastMove, KEYUP, game->bombs);
                    break;

                case SDLK_s:
                        move(game->p1, &newMove, &lastMove, KEYDOWN, game->bombs);
                    break;

                case SDLK_a:
                        move(game->p1, &newMove, &lastMove, KEYLEFT, game->bombs);
                    break;

                case SDLK_d:
                        move(game->p1, &newMove, &lastMove, KEYRIGHT, game->bombs);
                    break;

                case SDLK_SPACE:
                        bombPlacement(game->p1, game->bombs, game->renderer);
                    break;
                }

            }
        }
        handlePlayerExplosionCollision(game);
          
        SDL_RenderClear(game->renderer);
        SDL_RenderCopy(game->renderer, game->background, NULL, NULL);
        renderBoxes(game);
        renderBombsAndExplosions(game);

        if (game->p1->isHurt) {
            SDL_SetTextureColorMod(game->p1->texture, 255, 0, 0);           // Character turns red if hurt
        }
        else {
            SDL_SetTextureColorMod(game->p1->texture, 255, 255, 255);       // Restore color if not hurt
        }

        SDL_RenderCopy(game->renderer, game->p1->texture, &game->p1->clip[game->p1->currentFrame], &game->p1->pos);
        SDL_RenderCopy(game->renderer, game->p2->texture, &game->p2->clip[game->p2->currentFrame], &game->p2->pos);
        SDL_RenderCopy(game->renderer, game->p3->texture, &game->p3->clip[game->p3->currentFrame], &game->p3->pos);
        SDL_RenderCopy(game->renderer, game->p4->texture, &game->p4->clip[game->p4->currentFrame], &game->p4->pos);
        SDL_RenderPresent(game->renderer);
    }
}

PRIVATE bool initWinRen(Game game) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
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

    game->bitmapSurface = IMG_Load("resources/Background.png");                      //Laddar upp bakgrundsbilden till bitmapSurface (kanske m�ste �ndra bildens position)
    if (!game->bitmapSurface) {
        printf("Could not load Background to bitmapSurface: %s\n", IMG_GetError());
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

    game->bitmapSurface = IMG_Load("resources/Box.png");                      //Laddar upp bakgrundsbilden till bitmapSurface (kanske m�ste �ndra bildens position)
    if (!game->bitmapSurface) {
        printf("Could not load Box to bitmapSurface: %s\n", IMG_GetError());
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
    SDL_DestroyTexture(game->p1->texture);
    SDL_DestroyTexture(game->p2->texture);
    SDL_DestroyTexture(game->p3->texture);
    SDL_DestroyTexture(game->p4->texture);
    free(game->p1);
    free(game->p2);
    free(game->p3);
    free(game->p4);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    IMG_Quit();
    SDL_Quit();
}

PRIVATE void renderBoxes(Game game) {
     //// RENDERAR L�DORNA, INTE OPTIMERAT ////
     game->boxPos.w = 64;                  //Utanf�r loopen, alltid samma v�rde (h�jd/bredd p� l�dan)
     game->boxPos.h = 64;
     for (int row = 0; row < ROW_SIZE; row++) {
         for (int column = 0; column < COLUMN_SIZE; column++) {
             if (activeBox[row][column] == 1) {
                 game->boxPos.x = column * 64 + 64;
                 game->boxPos.y = row * 64 + 64;
                 SDL_RenderCopyEx(game->renderer, game->box, NULL, &game->boxPos, 0, NULL, SDL_FLIP_NONE);       // Renderar en l�da i taget
             }
         }
     }
 }


