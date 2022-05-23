#include "game.h"
#include <stdio.h>
#include <stdbool.h>

#define PUBLIC /* empty */
#define PRIVATE static

#define KEYDOWN 's'
#define KEYUP 'w'
#define KEYRIGHT 'd'
#define KEYLEFT 'a'

typedef struct ResetGameCallbackArgs {
    Game game;
    udpData packetData;
} ResetGameCallbackArgs;

PRIVATE bool initWinRen(Game game);
PRIVATE bool createStartMenu(Game game);
PRIVATE bool createBackground(Game game);
PUBLIC bool loadTextures(SDL_Renderer** renderer, SDL_Surface** bitmapSurface, SDL_Texture** texture, char pictureDestination[64]);
PRIVATE bool initNetwork(Network net, char inputIPAddress[]);
PRIVATE void sendUDPData(Network net, udpData packetData);
PRIVATE void receiveUDPData(Game game, Network net);
PRIVATE Uint32 resetGame(Uint32 interval, void *args);

PUBLIC Network createNet() {
    Network net = malloc(sizeof(struct NetworkData)); // create a memory in heap for network struct 
    return net; // return to main 
}

PUBLIC Game createGame() {
    Game game = malloc(sizeof(struct GameSettings)); // allocate memory 
    srand(time(0)); // make it possible to randomize powerups

    if (initWinRen(game))  { //init basics of SDL
        if (createStartMenu(game)) {
            if (createBackground(game)) {
               
                    int imgFlags = IMG_INIT_PNG;
                    if (!(IMG_Init(imgFlags) & imgFlags))
                    {
                        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
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
                                     // Start single player if no server response
    game->pIdx = 0;
    game->player[0] = createPlayer(1, 64, 64, game);
    game->player[1] = createPlayer(2, 960, 64, game);
    game->player[2] = createPlayer(3, 64, 698, game);
    game->player[3] = createPlayer(4, 960, 698, game);
    game->activePlayers = 4;
    game->boxes = createBoxes(game); 
    game->power = createPowers(game);
    initBombs(game->bombs);                           // Sets all bombs to NULL
    game->accessToServer = false;
    game->allPlayersDead = false;
    game->resetInitiated = false;
    game->playersDead = 0;
    game->inMenu = true;

    return game;
}

PUBLIC void updateGame(Game game, Network net, udpData packetData) {
    bool running = true;
    int newMove = 1, lastMove = 0;
    int frames = 0;                 // Used for character refresh rate in gameLogic.c
    bool inInstructions = false;
    int mousePos_x, mousePos_y;
    bool menuOptionSelected[MENUOPTIONS] = { 0,0,0,0 };
    const Uint8* currentKeyStates;
    bool resetIpAddress = true;

    while (running) {
        while (SDL_PollEvent(&game->event) != 0) {
            if (game->event.type == SDL_QUIT) {
                running = false;
            }
            if (!game->inMenu) {
                if (playerIsAlive(game->player[game->pIdx])) {
                    if (game->event.type == SDL_KEYDOWN) {
                        if (game->event.key.keysym.sym == SDLK_SPACE) {                     // Space intryckt (gamla sättet som pausar i 1 sek när man håller in knappen)
                            bombPlacement(game->player[game->pIdx], game->bombs, game->pIdx, game->renderer, net, packetData);
                            // removeBox(game->p1, game->boxes->activeBox);
                        }
                    }
                }
            }
            else {
                mousePos_x = game->event.motion.x;
                mousePos_y = game->event.motion.y;
                if (!inInstructions) {

                    // SERVER CONNECTION //
                    if (resetIpAddress) {
                        resetIpAddress = false;
                        memset(net->inputIPAddress, '\0', strlen(net->inputIPAddress));
                        printf("IP Address: ");
                    }
                    if (game->event.type == SDL_TEXTINPUT) {
                        strcat(net->inputIPAddress, game->event.text.text);
                        printf("%s", game->event.text.text);
                    }
                    if (game->event.type == SDL_KEYDOWN) {
                        if (game->event.key.keysym.sym == SDLK_BACKSPACE && strlen(net->inputIPAddress) > 0) {
                            net->inputIPAddress[strlen(net->inputIPAddress) - 1] = '\0';
                            printf("\b");
                        }
                        if ((game->event.key.keysym.sym == SDLK_KP_ENTER || game->event.key.keysym.sym == SDLK_RETURN) && net->inputIPAddress[0] != '\0') {
                            printf("\nConnecting to server: %s\n", net->inputIPAddress);
                            initNetwork(net, net->inputIPAddress);
                            memset(net->inputIPAddress, '\0', strlen(net->inputIPAddress));

                            if (!net->willSend) {
                                // Send arbitrary data to server so server acknowledges client
                                sprintf((char*)net->packet1->data, "%d\n", 99);
                                net->packet1->address.host = net->srvAddr.host;	                    // Set the destination host 
                                net->packet1->address.port = net->srvAddr.port;	                    // And destination port
                                net->packet1->len = strlen((char*)net->packet1->data) + 1;
                                SDLNet_UDP_Send(net->sd, -1, net->packet1);

                                SDL_Delay(1000);

                                // Assign correct player number based on server response
                                if (SDLNet_UDP_Recv(net->sd, net->packet1)) {
                                    printf("Packet received from server.\n");
                                    game->accessToServer = true;

                                    int playerIdx;
                                    sscanf((char*)net->packet1->data, "%d\n", &playerIdx);
                                    game->pIdx = playerIdx;
                                    packetData->pIdx = playerIdx;

                                    switch (playerIdx) {
                                    case 0:
                                        game->player[0] = createPlayer(1, 64, 64, game);
                                        game->activePlayers = 1;
                                        break;
                                    case 1:
                                        game->player[1] = createPlayer(2, 960, 64, game);
                                        game->activePlayers = 2;
                                        break;
                                    case 2:
                                        game->player[2] = createPlayer(3, 64, 698, game);
                                        game->activePlayers = 3;
                                        break;
                                    case 3:
                                        game->player[3] = createPlayer(4, 960, 698, game);
                                        game->activePlayers = 4;
                                        break;
                                    default: break;
                                    }
                                }
                                else {
                                    SDLNet_FreePacket(net->packet1);
                                    printf("Could not connect to the server\nIPAddress: ");
                                }
                            }
                            else {
                                SDLNet_FreePacket(net->packet1);
                                printf("Could not find the server\nIPAddress: ");
                            }
                        }
                    }
                    // SERVER CONNECTION END //

                    for (int i = 0; i < MENUOPTIONS; i++) {
                        if (mousePos_x >= 440 && mousePos_x <= 650 && mousePos_y >= 430 && mousePos_y <= 505) {         // Om musen är på "PLAY"
                            if (!menuOptionSelected[0]) {
                                menuOptionSelected[0] = true;
                            }
                            if (game->event.type == SDL_MOUSEBUTTONDOWN) {
                                game->inMenu = false;
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

        if (!game->inMenu) {
            currentKeyStates = SDL_GetKeyboardState(NULL);
            if (playerIsAlive(game->player[game->pIdx])) {
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

            if (game->accessToServer) {
                sendUDPData(net, packetData);
                receiveUDPData(game, net);
            }

            handlePlayerExplosionCollision(game, net, packetData);
            SDL_RenderClear(game->renderer);
            SDL_RenderCopy(game->renderer, game->background, NULL, NULL);
            renderPowerUps(game);
            renderBoxes(game);
            renderBombsAndExplosions(game, net, packetData);
            renderPlayers(game);
            if (game->accessToServer) {
                if (!game->allPlayersDead) {
                    if (!playerIsAlive(game->player[game->pIdx]) && !(game->activePlayers == (game->playersDead + 1))) {
                        SDL_RenderCopy(game->renderer, game->dead, NULL, NULL);
                    }
                    if ((game->activePlayers > 1) && (game->activePlayers == (game->playersDead + 1))) {
                        if (playerIsAlive(game->player[0])) {
                            SDL_RenderCopy(game->renderer, game->player1Wins, NULL, NULL);
                        }
                        else if (playerIsAlive(game->player[1])) {
                            SDL_RenderCopy(game->renderer, game->player2Wins, NULL, NULL);
                        }
                        else if (playerIsAlive(game->player[2])) {
                            SDL_RenderCopy(game->renderer, game->player3Wins, NULL, NULL);
                        }
                        else if (playerIsAlive(game->player[3])) {
                            SDL_RenderCopy(game->renderer, game->player4Wins, NULL, NULL);
                        }

                        if (!game->resetInitiated) {
                            ResetGameCallbackArgs *resetCargs = malloc(sizeof(ResetGameCallbackArgs));
                            resetCargs->game = game;
                            resetCargs->packetData = packetData;
                            game->resetGameTime = SDL_AddTimer(5000, resetGame, resetCargs);
                            game->resetInitiated = true;
                        }
                    }
                }
                else {
                    if (playerIsLastPlayer(game->player[0])) {
                        SDL_RenderCopy(game->renderer, game->player1Wins, NULL, NULL);
                    }
                    else if (playerIsLastPlayer(game->player[1])) {
                        SDL_RenderCopy(game->renderer, game->player2Wins, NULL, NULL);
                    }
                    else if (playerIsLastPlayer(game->player[2])) {
                        SDL_RenderCopy(game->renderer, game->player3Wins, NULL, NULL);
                    }
                    else if (playerIsLastPlayer(game->player[3])) {
                        SDL_RenderCopy(game->renderer, game->player4Wins, NULL, NULL);
                    } 
                }

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
    static int timer = 0;                               // För att räkna hur många paket som skickas per sekund. (Bara ett test, kan raderas när spelet/nätverket är helt klart)
    static int counter = 0;

    if (net->willSend) {
        sprintf((char *)net->packet1->data, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", packetData->pIdx, packetData->xPos, packetData->yPos, packetData->frame, packetData->isHurt, packetData->isDead, packetData->powerUpRow, packetData->powerUpCol, packetData->bombDropped, packetData->bombPosX, packetData->bombPosY, packetData->explosionRange, packetData->leftBoxVal, packetData->leftBoxRow, packetData->leftBoxCol, packetData->powerUpTaken, packetData->rightBoxVal, packetData->rightBoxRow, packetData->rightBoxCol, packetData->topBoxVal, packetData->topBoxRow, packetData->topBoxCol, packetData->bottomBoxVal, packetData->bottomBoxRow, packetData->bottomBoxCol);
        net->packet1->address.host = net->srvAddr.host;	                    // Set the destination host
        net->packet1->address.port = net->srvAddr.port;	                    // And destination port
        net->packet1->len = strlen((char *)net->packet1->data) + 1;
        SDLNet_UDP_Send(net->sd, -1, net->packet1);
        net->willSend = false;
        packetData->powerUpTaken = 0;
        packetData->bombDropped = 0;
        packetData->leftBoxVal = -1;
        packetData->rightBoxVal = -1;
        packetData->topBoxVal = -1;
        packetData->bottomBoxVal = -1;

        counter++;                                      // För att räkna hur många paket som skickas per sekund
        if (SDL_GetTicks() >= timer + 1000) {           // För att räkna hur många paket som skickas per sekund
            timer = SDL_GetTicks();
            // printf("Sent %d packets in 1 second\n", counter);
            counter = 0;
        }
    }
}

PRIVATE void receiveUDPData(Game game, Network net) {
    if (SDLNet_UDP_Recv(net->sd, net->packet2)){
        int idx, x, y, currentFrame, activePlayers, isHurt, isDead, powerUpRow, powerUpCol, bombDropped, bombPosX, bombPosY, explosionRange, leftBoxVal, leftBoxRow, leftBoxCol, powerUpTaken, rightBoxVal, rightBoxRow, rightBoxCol, topBoxVal, topBoxRow, topBoxCol, bottomBoxVal, bottomBoxRow, bottomBoxCol;
        sscanf((char * )net->packet2->data, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", &idx, &x, &y, &currentFrame, &activePlayers, &isHurt, &isDead, &powerUpRow, &powerUpCol, &bombDropped, &bombPosX, &bombPosY, &explosionRange, &leftBoxVal, &leftBoxRow, &leftBoxCol, &powerUpTaken, &rightBoxVal, &rightBoxRow, &rightBoxCol, &topBoxVal, &topBoxRow, &topBoxCol, &bottomBoxVal, &bottomBoxRow, &bottomBoxCol);
        
        if (activePlayers > game->activePlayers) {
            switch (activePlayers) {
            case 1: game->player[game->activePlayers] = createPlayer(game->activePlayers + 1, 64, 64, game);
                break;
            case 2: game->player[game->activePlayers] = createPlayer(game->activePlayers + 1, 960, 64, game);
                break;
            case 3: game->player[game->activePlayers] = createPlayer(game->activePlayers + 1, 64, 698, game);
                break;
            case 4: game->player[game->activePlayers] = createPlayer(game->activePlayers + 1, 960, 698, game);
                break;
            }
            (game->activePlayers)++;
        } 
        else {
            setPlayerPosX(game->player[idx], x);
            setPlayerPosY(game->player[idx], y);
            setPlayerFrame(game->player[idx], currentFrame);
            setPlayerExpRange(game->player[idx], explosionRange);
        }
        if (bombDropped) {
            // printf("Bomb dropped\n");
            uint8_t bombIdx = getBombIdx(game->bombs);
            BombTimerCallbackArgs *callbackArgs = malloc(sizeof(BombTimerCallbackArgs));
            callbackArgs->bomb = game->bombs[bombIdx] = createBomb(x, y, idx, game->renderer, explosionRange);
            callbackArgs->pIdx = game->pIdx;
            game->bombs[bombIdx]->redBombTime = SDL_AddTimer(2000, redBomb, callbackArgs);
            game->bombs[bombIdx]->bombTime = SDL_AddTimer(3000, explodeBomb, callbackArgs);
            game->bombs[bombIdx]->deleteBombTime = SDL_AddTimer(4000, explosionDoneClient, callbackArgs);
        }
        if (leftBoxVal >= 0) {
            activePowers[leftBoxRow][leftBoxCol] = leftBoxVal; 
        }
        if (rightBoxVal >= 0) {
            activePowers[rightBoxRow][rightBoxCol] = rightBoxVal; 
        }
        if (topBoxVal >= 0) {
            activePowers[topBoxRow][topBoxCol] = topBoxVal; 
        }
        if (bottomBoxVal >= 0) {
            activePowers[bottomBoxRow][bottomBoxCol] = bottomBoxVal; 
        }
        if (powerUpTaken) {
            activePowers[powerUpRow][powerUpCol] = 0;
        }
        if (isHurt) {
            setPlayerToHurt(game->player[idx]);

            if (isDead) {
                setPlayerToDead(game->player[idx]);
                game->playersDead++;

                if (game->activePlayers == game->playersDead) {
                    game->allPlayersDead = true;
                    setToLastPlayer(game->player[idx]);
                }
            }
        }
        else {
            setPlayerToNotHurt(game->player[idx]);
        }
    }
}

PRIVATE Uint32 resetGame(Uint32 interval, void *args) {
    ResetGameCallbackArgs *cArgs = (ResetGameCallbackArgs*) args;
    
    cArgs->game->inMenu = true;
    cArgs->game->resetInitiated = false;
    cArgs->game->allPlayersDead = false;
    cArgs->game->playersDead = 0;
    cArgs->packetData->isHurt = 0;
    cArgs->packetData->isDead = 0;
    cArgs->packetData->explosionRange = 2;

    for (int i = 0; i < cArgs->game->activePlayers; i++) {
        setPlayerToNotHurt(cArgs->game->player[i]);
        setPlayerLives(cArgs->game->player[i], 3);
        setPlayerToAlive(cArgs->game->player[i]);
        setBombsAvailable(cArgs->game->player[i], 1);
        setPlayerSpeed(cArgs->game->player[i], 2);
        setPlayerExpRange(cArgs->game->player[i], 2);
        setToNotLastPlayer(cArgs->game->player[i]);
    }

    if (cArgs->game->activePlayers >= 1) {
        setPlayerPosX(cArgs->game->player[0], 64);
        setPlayerPosY(cArgs->game->player[0], 64);
        setPlayerFrame(cArgs->game->player[0], 8);
    }
    if (cArgs->game->activePlayers >= 2) {
        setPlayerPosX(cArgs->game->player[1], 960);
        setPlayerPosY(cArgs->game->player[1], 64);
        setPlayerFrame(cArgs->game->player[1], 12);
    }
    if (cArgs->game->activePlayers >= 3) {
        setPlayerPosX(cArgs->game->player[2], 64);
        setPlayerPosY(cArgs->game->player[2], 698);
        setPlayerFrame(cArgs->game->player[2], 8);
    }
    if (cArgs->game->activePlayers >= 4) {
        setPlayerPosX(cArgs->game->player[3], 960);
        setPlayerPosY(cArgs->game->player[3], 698);
        setPlayerFrame(cArgs->game->player[3], 12);
    }

    for (int i = 0; i < ROW_SIZE; i++) {
        for (int j = 0; j < COLUMN_SIZE; j++) {
            activeBox[i][j] = resetBoxPos[i][j];
        }
    }

    free(cArgs);
    return 0;
}

PRIVATE bool initNetwork(Network net, char inputIPAddress[]) {
    net->willSend = false;

    if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init Error: %s\n", SDLNet_GetError());
        net->willSend = true;
	}

    if (!(net->sd = SDLNet_UDP_Open(0))) // UDPSOCKET
	{
		fprintf(stderr, "SDLNet_UDP_Open Error: %s\n", SDLNet_GetError());
        net->willSend = true;
	}

    /* Resolve server name  */
	if (SDLNet_ResolveHost(&net->srvAddr, inputIPAddress, 2000) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000) Error: %s\n", SDLNet_GetError());
        net->willSend = true;
	}

    if (!((net->packet1 = SDLNet_AllocPacket(512)) && (net->packet2 = SDLNet_AllocPacket(512))))
	{
		fprintf(stderr, "SDLNet_AllocPacket Error: %s\n", SDLNet_GetError());
        net->willSend = true;
	}

    return net->willSend;
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
    } // create 

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

    SDL_strlcpy(pictureDestination, "resources/player1won.png", sizeof pictureDestination);
    loadTextures(&game->renderer, &game->bitmapSurface, &game->player1Wins, pictureDestination);

    SDL_strlcpy(pictureDestination, "resources/player2won.png", sizeof pictureDestination);
    loadTextures(&game->renderer, &game->bitmapSurface, &game->player2Wins, pictureDestination);

    SDL_strlcpy(pictureDestination, "resources/player3won.png", sizeof pictureDestination);
    loadTextures(&game->renderer, &game->bitmapSurface, &game->player3Wins, pictureDestination);

    SDL_strlcpy(pictureDestination, "resources/player4won.png", sizeof pictureDestination);
    loadTextures(&game->renderer, &game->bitmapSurface, &game->player4Wins, pictureDestination);

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
    //SDLNet_FreePacket(net->packet1);
    //SDLNet_FreePacket(net->packet2);
    free(packetData);
    //free(net);
	SDLNet_Quit();

    for (uint8_t i = 0; i < game->activePlayers; i++) {
        destroyPlayerTexture(game->player[i]);
        free(game->player[i]);
    }

    SDL_DestroyTexture(game->power->biggerExplosions);
    SDL_DestroyTexture(game->power->moreBombs);
    SDL_DestroyTexture(game->power->biggerExplosions);
    SDL_DestroyTexture(game->boxes->box);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    free(game);
    IMG_Quit();
    SDL_Quit();
}


PUBLIC udpData createPacketData(Game game) {
    udpData packetData = malloc(sizeof(struct udpData));
    packetData->pIdx = game->pIdx;
    packetData->xPos = 0;
    packetData->yPos = 0;
    packetData->frame = 0;
    packetData->isHurt = 0;
    packetData->isDead = 0;
    packetData->bombDropped = 0;
    packetData->bombPosX = 0;
    packetData->bombPosY = 0;
    packetData->explosionRange = getPlayerExpRange(game->player[game->pIdx]);
    packetData->powerUpTaken = 0;
    packetData->leftBoxVal = -1;
    packetData->rightBoxVal = -1;
    packetData->topBoxVal = -1;
    packetData->bottomBoxVal = -1;

    packetData->bottomBoxCol = 0;
    packetData->bottomBoxRow = 0; 
    packetData->leftBoxCol = 0;
    packetData->leftBoxRow = 0; 
    packetData->rightBoxCol = 0;
    packetData->rightBoxRow = 0;
    packetData->topBoxCol = 0; 
    packetData->topBoxRow = 0; 
    return packetData;
}
