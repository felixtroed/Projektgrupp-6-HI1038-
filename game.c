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
PRIVATE bool createStartMenu(Game game);
PRIVATE bool createBackground(Game game);
PRIVATE bool showBoxes(Game game);
PRIVATE void renderBoxes(Game game);
PUBLIC bool loadTextures(SDL_Renderer** renderer, SDL_Surface** bitmapSurface, SDL_Texture** texture, char pictureDestination[64]);

PUBLIC Game createGame() {
    Game game = malloc(sizeof(struct GameSettings));

    if (initWinRen(game)) {
        if (createStartMenu(game)) {
            if (createBackground(game)) {
                if (showBoxes(game)) {
                    int imgFlags = IMG_INIT_PNG;
                    if (!(IMG_Init(imgFlags) & imgFlags))
                    {
                        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    }
                }
            }
        }
    }
    game->menuOptionPos[0].x = 441;             // 'PLAY' button in menu
    game->menuOptionPos[0].y = 429;
    game->menuOptionPos[0].w = 212;
    game->menuOptionPos[0].h = 71;

    game->menuOptionPos[1].x = 336;             // 'CONTROLS' button in menu
    game->menuOptionPos[1].y = 521;
    game->menuOptionPos[1].w = 419;
    game->menuOptionPos[1].h = 71;

    game->menuOptionPos[2].x = 470;             // 'QUIT' button in menu
    game->menuOptionPos[2].y = 613;
    game->menuOptionPos[2].w = 151;
    game->menuOptionPos[2].h = 71;

    game->menuOptionPos[3].x = 39;             // 'BACK' button in menu
    game->menuOptionPos[3].y = 725;
    game->menuOptionPos[3].w = 212;
    game->menuOptionPos[3].h = 71;

    game->p1 = createPlayer(1, 64, 64, game);
    game->p2 = createPlayer(2, 960, 64, game);
    game->p3 = createPlayer(3, 64, 704, game);
    game->p4 = createPlayer(4, 960, 704, game);
    // game->boxes = createBoxes(game);
    initBombs(game->bombs);                           // Sets all bombs to NULL

    return game;
}

PUBLIC void updateGame(Game game) {
    bool running = true;
    int newMove = 1, lastMove = 0;
    int frames = 0;                 // Used for character refresh rate in gameLogic.c
    bool inMenu = true;
    bool inControls = false;
    int mousePos_x, mousePos_y;
    bool menuOptionSelected[MENUOPTIONS] = { 0,0,0,0 };
    const Uint8* currentKeyStates;

    while (running) {
        while (SDL_PollEvent(&game->event) != 0) {
            if (game->event.type == SDL_QUIT) {
                running = false;
            }
            if (!inMenu) {
                if (game->p1->isAlive) {
                    if (game->event.type == SDL_KEYDOWN) {
                        if (game->event.key.keysym.sym == SDLK_SPACE) {                     // Space intryckt (gamla sättet som pausar i 1 sek när man håller in knappen)
                            bombPlacement(game->p1, game->bombs, game->renderer);
                            // removeBox(game->p1, game->boxes->activeBox);
                        }
                    }
                }
            }
            else {
                mousePos_x = game->event.motion.x;
                mousePos_y = game->event.motion.y;
                if (!inControls) {
                    for (int i = 0; i < MENUOPTIONS; i++) {
                        if (mousePos_x >= 440 && mousePos_x <= 650 && mousePos_y >= 430 && mousePos_y <= 505) {         // Om musen är på "PLAY"
                            if (!menuOptionSelected[0]) {
                                menuOptionSelected[0] = true;
                            }
                            if (game->event.type == SDL_MOUSEBUTTONDOWN) {
                                inMenu = false;
                            }
                        }
                        else {
                            if (menuOptionSelected[0]) {
                                menuOptionSelected[0] = false;
                            }
                        }

                        if (mousePos_x >= 336 && mousePos_x <= 750 && mousePos_y >= 525 && mousePos_y <= 596) {         // Om musen är på "CONTROLS"
                            if (!menuOptionSelected[1]) {
                                menuOptionSelected[1] = true;
                            }
                            if (game->event.type == SDL_MOUSEBUTTONDOWN) {
                                inControls = true;
                                menuOptionSelected[1] = false;
                            }
                        }
                        else {
                            if (menuOptionSelected[1]) {
                                menuOptionSelected[1] = false;
                            }
                        }

                        if (mousePos_x >= 470 && mousePos_x <= 615 && mousePos_y >= 615 && mousePos_y <= 690) {         // Om musen är på "QUIT"
                            if (!menuOptionSelected[2]) {
                                menuOptionSelected[2] = true;
                            }
                            if (game->event.type == SDL_MOUSEBUTTONDOWN) {
                                running = false;
                            }
                        }
                        else {
                            if (menuOptionSelected[2]) {
                                menuOptionSelected[2] = false;
                            }
                        }
                    }
                }
                else {
                    if (mousePos_x >= 40 && mousePos_x <= 242 && mousePos_y >= 726 && mousePos_y <= 800) {              // Om musen är på "BACK"
                        if (!menuOptionSelected[3]) {
                            menuOptionSelected[3] = true;
                        }
                        if (game->event.type == SDL_MOUSEBUTTONDOWN) {
                            inControls = false;
                        }
                    }
                    else {
                        if (menuOptionSelected[3]) {
                            menuOptionSelected[3] = false;
                        }
                    }
                }
            }
            
        }

        if (!inMenu) {
            currentKeyStates = SDL_GetKeyboardState(NULL);
            if (game->p1->isAlive) {
                if (currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP]) {                // Funkar för både WASD och pilar
                    move(game->p1, &newMove, &lastMove, KEYUP, game->bombs, &frames);
                }
                else if (currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN]) {
                    move(game->p1, &newMove, &lastMove, KEYDOWN, game->bombs, &frames);
                }
                else if (currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]) {
                    move(game->p1, &newMove, &lastMove, KEYLEFT, game->bombs, &frames);
                }
                else if (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT]) {
                    move(game->p1, &newMove, &lastMove, KEYRIGHT, game->bombs, &frames);
                }
            }

            handlePlayerExplosionCollision(game);

            SDL_RenderClear(game->renderer);
            SDL_RenderCopy(game->renderer, game->background, NULL, NULL);
            renderBoxes(game);
            renderBombsAndExplosions(game);
            renderPlayers(game);
            SDL_RenderPresent(game->renderer);
        }
        else {
            SDL_RenderClear(game->renderer);
            SDL_RenderCopy(game->renderer, game->startMenu, NULL, NULL);
            if (inControls) {
                SDL_RenderCopy(game->renderer, game->controlsMenu, NULL, NULL);
                if (menuOptionSelected[3]) {
                    SDL_RenderCopy(game->renderer, game->redBack, NULL, &game->menuOptionPos[3]);
                }
            }
            if (menuOptionSelected[0]) {
                SDL_RenderCopy(game->renderer, game->redPlay, NULL, &game->menuOptionPos[0]);
            }
            else if (menuOptionSelected[1]) {
                SDL_RenderCopy(game->renderer, game->redControls, NULL, &game->menuOptionPos[1]);
            }
            else if (menuOptionSelected[2]) {
                SDL_RenderCopy(game->renderer, game->redQuit, NULL, &game->menuOptionPos[2]);
            }
            SDL_RenderPresent(game->renderer);
        }
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

    char pictureDestination[64] = "resources/Background.png";
    loadTextures(&game->renderer, &game->bitmapSurface, &game->background, pictureDestination);

    return true;
}

PRIVATE bool createStartMenu(Game game) {

    char pictureDestination[64];

    strcpy_s(pictureDestination, sizeof pictureDestination, "resources/Menu.png");
    loadTextures(&game->renderer, &game->bitmapSurface, &game->startMenu, pictureDestination);

    strcpy_s(pictureDestination, sizeof pictureDestination, "resources/Controls-Menu.png");
    loadTextures(&game->renderer, &game->bitmapSurface, &game->controlsMenu, pictureDestination);

    strcpy_s(pictureDestination, sizeof pictureDestination, "resources/Play.png");
    loadTextures(&game->renderer, &game->bitmapSurface, &game->redPlay, pictureDestination);

    strcpy_s(pictureDestination, sizeof pictureDestination, "resources/Controls.png");
    loadTextures(&game->renderer, &game->bitmapSurface, &game->redControls, pictureDestination);

    strcpy_s(pictureDestination, sizeof pictureDestination, "resources/Quit.png");
    loadTextures(&game->renderer, &game->bitmapSurface, &game->redQuit, pictureDestination);

    strcpy_s(pictureDestination, sizeof pictureDestination, "resources/Back.png");
    loadTextures(&game->renderer, &game->bitmapSurface, &game->redBack, pictureDestination);

    return true;
}

PRIVATE bool showBoxes(Game game) {

    char boxDestination[64] = "resources/Box.png";
    loadTextures(&game->renderer, &game->bitmapSurface, &game->box, boxDestination);

    return true;
}

PUBLIC bool loadTextures(SDL_Renderer** renderer, SDL_Surface** bitmapSurface, SDL_Texture** texture, char pictureDestination[64]) {
    *bitmapSurface = IMG_Load(pictureDestination);
    if (!(*bitmapSurface)) {
        printf("Could not load %s to bitmapSurface: %s\n", pictureDestination, IMG_GetError());
        return false;
    }

    *texture = SDL_CreateTextureFromSurface(*renderer, *bitmapSurface);
    if (!(*texture)) {
        printf("Could not create texture from bitmapSurface: %s\n", SDL_GetError());
        return false;
    }

    SDL_FreeSurface(*bitmapSurface);                                           //Raderar bitmapSurface (frig�r minnet) (Background finns fortfarande kvar)
    if (!(*bitmapSurface)) {
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


