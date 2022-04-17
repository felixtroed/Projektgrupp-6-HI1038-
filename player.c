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
    player->clip[3].h = PLAYER_HEIGHT;
 }