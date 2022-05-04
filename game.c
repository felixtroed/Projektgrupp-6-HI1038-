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
PRIVATE void initNetwork(Network net);
PRIVATE void sendUDPData(Network net, udpData packetData);
PRIVATE void receiveUDPData(Game game, Network net);

PUBLIC Network createNet() {
    Network net = malloc(sizeof(struct NetworkData));
    initNetwork(net);
    return net;
}

PUBLIC Game createGame(Network net) {
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

    game->menuOptionPos[1].x = 258;             // 'INSTRUCTIONS' button in menu
    game->menuOptionPos[1].y = 521;
    game->menuOptionPos[1].w = 578;
    game->menuOptionPos[1].h = 71;

    game->menuOptionPos[2].x = 470;             // 'QUIT' button in menu
    game->menuOptionPos[2].y = 613;
    game->menuOptionPos[2].w = 151;
    game->menuOptionPos[2].h = 71;

    game->menuOptionPos[3].x = 39;             // 'BACK' button in menu
    game->menuOptionPos[3].y = 725;
    game->menuOptionPos[3].w = 212;
    game->menuOptionPos[3].h = 71;

    // Send arbitrary data to server so server acknowledges client
    sprintf((char *)net->packet1->data, "%d\n", 99);
    net->packet1->address.host = net->srvAddr.host;	                    /* Set the destination host */
    net->packet1->address.port = net->srvAddr.port;	                    /* And destination port */
    net->packet1->len = strlen((char *)net->packet1->data) + 1;
    SDLNet_UDP_Send(net->sd, -1, net->packet1);
    
    SDL_Delay(1000);                                                    // Wait some time for server to send data back

    // Assign correct player number based on server response
    if (SDLNet_UDP_Recv(net->sd, net->packet1)) {
        printf("Packet received from server.\n");

        int playerIdx;
        sscanf((char * )net->packet1->data, "%d\n", &playerIdx);
        game->pIdx = playerIdx;

        switch (game->pIdx) {
            case 0:
                game->player[0] = createPlayer(1, 64, 64, game);
                game->activePlayers = 1;
                break;
            case 1:
                game->player[0] = createPlayer(1, 64, 64, game);
                game->player[1] = createPlayer(2, 960, 64, game);
                game->activePlayers = 2;
                break;
            case 2:
                game->player[0] = createPlayer(1, 64, 64, game);
                game->player[1] = createPlayer(2, 960, 64, game);
                game->player[2] = createPlayer(3, 64, 640, game);
                game->activePlayers = 3;
                break;
            case 3:
                game->player[0] = createPlayer(1, 64, 64, game);
                game->player[1] = createPlayer(2, 960, 64, game);
                game->player[2] = createPlayer(3, 64, 640, game);
                game->player[3] = createPlayer(4, 960, 640, game);
                game->activePlayers = 4;
                break;
            default: break;
        }
    }
    else {                                                              // Start single player if no server response
        game->pIdx = 0;
        game->player[0] = createPlayer(1, 64, 64, game);
        game->player[1] = createPlayer(2, 960, 64, game);
        game->player[2] = createPlayer(3, 64, 704, game);
        game->player[3] = createPlayer(4, 960, 704, game);
        game->activePlayers = 4;
    }

    // game->boxes = createBoxes(game);
    game->power = createPowers(game);
    initBombs(game->bombs);                           // Sets all bombs to NULL

    return game;
}

PUBLIC void updateGame(Game game, Network net, udpData packetData) {
    bool running = true;
    int newMove = 1, lastMove = 0;
    int frames = 0;                 // Used for character refresh rate in gameLogic.c
    bool inMenu = true;
    bool inInstructions = false;
    int mousePos_x, mousePos_y;
    bool menuOptionSelected[MENUOPTIONS] = { 0,0,0,0 };
    const Uint8* currentKeyStates;

    while (running) {
        while (SDL_PollEvent(&game->event) != 0) {
            if (game->event.type == SDL_QUIT) {
                running = false;
            }
            if (!inMenu) {
                if (game->player[game->pIdx]->isAlive) {
                    if (game->event.type == SDL_KEYDOWN) {
                        if (game->event.key.keysym.sym == SDLK_SPACE) {                     // Space intryckt (gamla sättet som pausar i 1 sek när man håller in knappen)
                            bombPlacement(game->player[game->pIdx], game->bombs, game->renderer);
                            // removeBox(game->p1, game->boxes->activeBox);
                        }
                    }
                }
            }
            else {
                mousePos_x = game->event.motion.x;
                mousePos_y = game->event.motion.y;
                if (!inInstructions) {
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

                        if (mousePos_x >= 259 && mousePos_x <= 828 && mousePos_y >= 525 && mousePos_y <= 595) {         // Om musen är på "INSTRUCTIONS"
                            if (!menuOptionSelected[1]) {
                                menuOptionSelected[1] = true;
                            }
                            if (game->event.type == SDL_MOUSEBUTTONDOWN) {
                                inInstructions = true;
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
                            inInstructions = false;
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
            if (game->player[game->pIdx]->isAlive) {
                if (currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP]) {                // Funkar för både WASD och pilar
                    move(game->player[game->pIdx], &newMove, &lastMove, KEYUP, game->bombs, &frames, net, packetData);
                }
                else if (currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN]) {
                    move(game->player[game->pIdx], &newMove, &lastMove, KEYDOWN, game->bombs, &frames, net, packetData);
                }
                else if (currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]) {
                    move(game->player[game->pIdx], &newMove, &lastMove, KEYLEFT, game->bombs, &frames, net, packetData);
                }
                else if (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT]) {
                    move(game->player[game->pIdx], &newMove, &lastMove, KEYRIGHT, game->bombs, &frames, net, packetData);
                }
            }

            sendUDPData(net, packetData);
            receiveUDPData(game, net);
            handlePlayerExplosionCollision(game, net, packetData);
            pickUpPowerUps(game->player[game->pIdx]);

            SDL_RenderClear(game->renderer);
            SDL_RenderCopy(game->renderer, game->background, NULL, NULL);
            renderPowerUps(game);
            renderBoxes(game);
            renderBombsAndExplosions(game);
            renderPlayers(game);
            if (!game->player[game->pIdx]->isAlive) {
                SDL_RenderCopy(game->renderer, game->dead, NULL, NULL);
            }
            SDL_RenderPresent(game->renderer);
        }
        else {
            SDL_RenderClear(game->renderer);
            SDL_RenderCopy(game->renderer, game->startMenu, NULL, NULL);
            if (inInstructions) {
                SDL_RenderCopy(game->renderer, game->instructionsMenu, NULL, NULL);
                if (menuOptionSelected[3]) {
                    SDL_RenderCopy(game->renderer, game->redBack, NULL, &game->menuOptionPos[3]);
                }
            }
            if (menuOptionSelected[0]) {
                SDL_RenderCopy(game->renderer, game->redPlay, NULL, &game->menuOptionPos[0]);
            }
            else if (menuOptionSelected[1]) {
                SDL_RenderCopy(game->renderer, game->redInstructions, NULL, &game->menuOptionPos[1]);
            }
            else if (menuOptionSelected[2]) {
                SDL_RenderCopy(game->renderer, game->redQuit, NULL, &game->menuOptionPos[2]);
            }
            SDL_RenderPresent(game->renderer);
        }
    }
}

PRIVATE void sendUDPData(Network net, udpData packetData) {
    if (net->willSend) {
        sprintf((char *)net->packet1->data, "%d %d %d %d %d %d\n", packetData->pIdx, packetData->xPos, packetData->yPos, packetData->frame, packetData->isHurt, packetData->isDead);
        net->packet1->address.host = net->srvAddr.host;	                    /* Set the destination host */
        net->packet1->address.port = net->srvAddr.port;	                    /* And destination port */
        net->packet1->len = strlen((char *)net->packet1->data) + 1;
        SDLNet_UDP_Send(net->sd, -1, net->packet1);    
        net->willSend = false;
    }
}

PRIVATE void receiveUDPData(Game game, Network net) {
    if (SDLNet_UDP_Recv(net->sd, net->packet2)){
        int idx, x, y, currentFrame, activePlayers, isHurt, isDead;
        sscanf((char * )net->packet2->data, "%d %d %d %d %d %d %d\n", &idx, &x, &y, &currentFrame, &activePlayers, &isHurt, &isDead);

        // printf("Active players: %d\n", activePlayers);
        // printf("Game->activePlayers: %d\n", game->activePlayers);

        if (activePlayers > game->activePlayers) {
            game->player[game->activePlayers] = createPlayer(game->activePlayers + 1, x, y, game);
            (game->activePlayers)++;
        }
        else {
            game->player[idx]->pos.x = x;
            game->player[idx]->pos.y = y;
            game->player[idx]->currentFrame = currentFrame;
        }
        if (isHurt) {
            game->player[idx]->isHurt = true;

            if (isDead) {
                game->player[idx]->isAlive = false;
            }
        }
        else {
            game->player[idx]->isHurt = false;
        }
    }
}

PRIVATE void initNetwork(Network net) {
    if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init Error: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    if (!(net->sd = SDLNet_UDP_Open(0)))
	{
		fprintf(stderr, "SDLNet_UDP_Open Error: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    /* Resolve server name  */
	if (SDLNet_ResolveHost(&net->srvAddr, "127.0.0.1", 2000) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000) Error: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    if (!((net->packet1 = SDLNet_AllocPacket(512)) && (net->packet2 = SDLNet_AllocPacket(512))))
	{
		fprintf(stderr, "SDLNet_AllocPacket Error: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    net->willSend = false;
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

    SDL_strlcpy(pictureDestination, "resources/Dead.png", sizeof pictureDestination);
    loadTextures(&game->renderer, &game->bitmapSurface, &game->dead, pictureDestination);

    return true;
}

PRIVATE bool createStartMenu(Game game) {

    char pictureDestination[64];

    SDL_strlcpy(pictureDestination, "resources/Menu.png", sizeof pictureDestination);
    loadTextures(&game->renderer, &game->bitmapSurface, &game->startMenu, pictureDestination);

    SDL_strlcpy(pictureDestination, "resources/Instructions-Menu.png", sizeof pictureDestination);
    loadTextures(&game->renderer, &game->bitmapSurface, &game->instructionsMenu, pictureDestination);

    SDL_strlcpy(pictureDestination, "resources/Play.png", sizeof pictureDestination);
    loadTextures(&game->renderer, &game->bitmapSurface, &game->redPlay, pictureDestination);

    SDL_strlcpy(pictureDestination, "resources/Instructions.png", sizeof pictureDestination);
    loadTextures(&game->renderer, &game->bitmapSurface, &game->redInstructions, pictureDestination);

    SDL_strlcpy(pictureDestination, "resources/Quit.png", sizeof pictureDestination);
    loadTextures(&game->renderer, &game->bitmapSurface, &game->redQuit, pictureDestination);

    SDL_strlcpy(pictureDestination, "resources/Back.png", sizeof pictureDestination);
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

PUBLIC void exitGame(Game game, Network net, udpData packetData) {
    SDLNet_FreePacket(net->packet1);
    SDLNet_FreePacket(net->packet2);
    free(packetData);
    free(net);
	SDLNet_Quit();

    for (uint8_t i = 0; i < game->activePlayers; i++) {
        SDL_DestroyTexture(game->player[i]->texture);
        free(game->player[i]);
    }

    SDL_DestroyTexture(game->power->biggerExplosions);
    SDL_DestroyTexture(game->power->moreBombs);
    SDL_DestroyTexture(game->power->biggerExplosions);

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

PUBLIC udpData createPacketData(uint8_t pIdx) {
    udpData packetData = malloc(sizeof(struct udpData));
    packetData->pIdx = pIdx;
    packetData->xPos = 0;
    packetData->yPos = 0;
    packetData->frame = 0;
    packetData->isHurt = 0;
    packetData->isDead = 0;

    return packetData;
}
