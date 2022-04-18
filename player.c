#include "player.h"

#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int PLAYER_WIDTH = 64;
PRIVATE int PLAYER_HEIGHT = 64;

PRIVATE void initClips(Player player);

PUBLIC Player createPlayer(int x, int y, Game game) {
    Player player = malloc(sizeof(struct PlayerSettings));

    player->texture = NULL;
    player->surface = IMG_Load("resources/old-man.png");
    if(player->surface == NULL ) {
            printf( "Unable to load image! SDL_image Error: %s\n", IMG_GetError());
    }
    else {
        // Create texture from surface pixels
        player->texture = SDL_CreateTextureFromSurface(game->renderer, player->surface);
        if(player->texture == NULL ) {
            printf( "Unable to create texture! SDL Error: %s\n", SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(player->surface);
        printf("Player surface freed.\n");
    }

    player->pos.x = x;
    player->pos.y = y;
    player->pos.w = PLAYER_WIDTH;
    player->pos.h = PLAYER_HEIGHT;
    player->currentFrame = 0;
    player->speed = 0;
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