#include "player.h"

#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int PLAYER_WIDTH = 64;
PRIVATE int PLAYER_HEIGHT = 64;

PRIVATE void initClips(Player player);
PUBLIC void renderPlayers(Game game);

PUBLIC Player createPlayer(int playerNumber, int x, int y, Game game) {
    Player player = malloc(sizeof(struct PlayerSettings));

    switch (playerNumber) {                                                 // Laddar upp rätt bild för rätt player
        case 1:
            player->surface = IMG_Load("resources/old-man.png");
            if (player->surface == NULL) {
                printf("Unable to load player 1 image! SDL_image Error: %s\n", IMG_GetError());
            }
            else {                                                          // Create texture from surface pixels
                player->texture = SDL_CreateTextureFromSurface(game->renderer, player->surface);
                if (player->texture == NULL) {
                    printf("Unable to create player 1 texture! SDL Error: %s\n", SDL_GetError());
                }
                SDL_FreeSurface(player->surface);                           // Get rid of old loaded surface
                player->currentFrame = 8;                                   // Player is turned right from start
            } break;

        case 2:
            player->surface = IMG_Load("resources/green-man.png");
            if (player->surface == NULL) {
                printf("Unable to load player 2 image! SDL_image Error: %s\n", IMG_GetError());
            }
            else {
                player->texture = SDL_CreateTextureFromSurface(game->renderer, player->surface);
                if (player->texture == NULL) {
                    printf("Unable to create player 2 texture! SDL Error: %s\n", SDL_GetError());
                }
                SDL_FreeSurface(player->surface);
                player->currentFrame = 12;                                  // Player is turned left from start
            } break;

        case 3:
            player->surface = IMG_Load("resources/blue-man.png");
            if (player->surface == NULL) {
                printf("Unable to load player 3 image! SDL_image Error: %s\n", IMG_GetError());
            }
            else {
                player->texture = SDL_CreateTextureFromSurface(game->renderer, player->surface);
                if (player->texture == NULL) {
                    printf("Unable to create player 3 texture! SDL Error: %s\n", SDL_GetError());
                }
                SDL_FreeSurface(player->surface);
                player->currentFrame = 8;
            } break;

        case 4:
            player->surface = IMG_Load("resources/red-man.png");
            if (player->surface == NULL) {
                printf("Unable to load player 4 image! SDL_image Error: %s\n", IMG_GetError());
            }
            else {
                player->texture = SDL_CreateTextureFromSurface(game->renderer, player->surface);
                if (player->texture == NULL) {
                    printf("Unable to create player 4 texture! SDL Error: %s\n", SDL_GetError());
                }
                SDL_FreeSurface(player->surface);
                player->currentFrame = 12;
            } break;
    }

    player->pos.x = x;
    player->pos.y = y;
    player->pos.w = PLAYER_WIDTH;
    player->pos.h = PLAYER_HEIGHT;
    player->hitboxPos.w = 34;            // The actual width of the character in each clip
    player->hitboxPos.h = 46;            // The actual height of the character in each clip
    player->speed = 2;
    player->bombsAvailable = 4;
    // player->explosionRange = 3;
    // player->numOfBoxes = 1;
    player->isHurt = false;
    player->isAlive = true;
    player->lifes = 20;
    initClips(player);
    return player;
}

PRIVATE void initClips(Player player) {
    player->clip[0].x = 0;
    player->clip[0].y = 0;
    player->clip[0].w = PLAYER_WIDTH;
    player->clip[0].h = PLAYER_HEIGHT;

    player->clip[1].x = 64;
    player->clip[1].y = 0;
    player->clip[1].w = PLAYER_WIDTH;
    player->clip[1].h = PLAYER_HEIGHT;

    player->clip[2].x = 128;
    player->clip[2].y = 0;
    player->clip[2].w = PLAYER_WIDTH;
    player->clip[2].h = PLAYER_HEIGHT;

    player->clip[3].x = 192;
    player->clip[3].y = 0;
    player->clip[3].w = PLAYER_WIDTH;
    player->clip[3].h = PLAYER_HEIGHT; // gubben g�r ner

    player->clip[4].x = 0;
    player->clip[4].y = 192;
    player->clip[4].w = PLAYER_WIDTH;
    player->clip[4].h = PLAYER_HEIGHT; 

    player->clip[5].x = 64;
    player->clip[5].y = 192;
    player->clip[5].w = PLAYER_WIDTH;
    player->clip[5].h = PLAYER_HEIGHT;

    player->clip[6].x = 128;
    player->clip[6].y = 192;
    player->clip[6].w = PLAYER_WIDTH;
    player->clip[6].h = PLAYER_HEIGHT;

    player->clip[7].x = 192;
    player->clip[7].y = 192;
    player->clip[7].w = PLAYER_WIDTH;
    player->clip[7].h = PLAYER_HEIGHT; // gubben g�r upp

    player->clip[8].x = 0;
    player->clip[8].y = 128;
    player->clip[8].w = PLAYER_WIDTH;
    player->clip[8].h = PLAYER_HEIGHT;

    player->clip[9].x = 64;
    player->clip[9].y = 128;
    player->clip[9].w = PLAYER_WIDTH;
    player->clip[9].h = PLAYER_HEIGHT;

    player->clip[10].x = 128;
    player->clip[10].y = 128;
    player->clip[10].w = PLAYER_WIDTH;
    player->clip[10].h = PLAYER_HEIGHT;

    player->clip[11].x = 192;
    player->clip[11].y = 128;
    player->clip[11].w = PLAYER_WIDTH;
    player->clip[11].h = PLAYER_HEIGHT; // gubben g�r h�ger

    player->clip[12].x = 0;
    player->clip[12].y = 64;
    player->clip[12].w = PLAYER_WIDTH;
    player->clip[12].h = PLAYER_HEIGHT;

    player->clip[13].x = 64;
    player->clip[13].y = 64;
    player->clip[13].w = PLAYER_WIDTH;
    player->clip[13].h = PLAYER_HEIGHT;

    player->clip[14].x = 128;
    player->clip[14].y = 64;
    player->clip[14].w = PLAYER_WIDTH;
    player->clip[14].h = PLAYER_HEIGHT;

    player->clip[15].x = 192;
    player->clip[15].y = 64;
    player->clip[15].w = PLAYER_WIDTH;
    player->clip[15].h = PLAYER_HEIGHT; // gubben g�r v�nster
 }

PUBLIC void renderPlayers(Game game) {
    if (game->player[0]->isAlive) {
        if (game->player[0]->isHurt)
            SDL_SetTextureColorMod(game->player[0]->texture, 255, 0, 0);           // Character turns red if hurt
        else {
            SDL_SetTextureColorMod(game->player[0]->texture, 255, 255, 255);       // Restore color if not hurt
        }

        SDL_RenderCopy(game->renderer, game->player[0]->texture, &game->player[0]->clip[game->player[0]->currentFrame], &game->player[0]->pos);
    }

    if (game->player[1]->isAlive) {
        if (game->player[1]->isHurt)
            SDL_SetTextureColorMod(game->player[1]->texture, 255, 0, 0);
        else {
            SDL_SetTextureColorMod(game->player[1]->texture, 255, 255, 255);
        }

        SDL_RenderCopy(game->renderer, game->player[1]->texture, &game->player[1]->clip[game->player[1]->currentFrame], &game->player[1]->pos);
    }

    if (game->player[2]->isAlive) {
        if (game->player[2]->isHurt)
            SDL_SetTextureColorMod(game->player[2]->texture, 255, 0, 0);
        else {
            SDL_SetTextureColorMod(game->player[2]->texture, 255, 255, 255);
        }

        SDL_RenderCopy(game->renderer, game->player[2]->texture, &game->player[2]->clip[game->player[2]->currentFrame], &game->player[2]->pos);
    }

    if (game->player[3]->isAlive) {
        if (game->player[3]->isHurt)
            SDL_SetTextureColorMod(game->player[3]->texture, 255, 0, 0);
        else {
            SDL_SetTextureColorMod(game->player[3]->texture, 255, 255, 255);
        }

        SDL_RenderCopy(game->renderer, game->player[3]->texture, &game->player[3]->clip[game->player[3]->currentFrame], &game->player[3]->pos);
    }
}