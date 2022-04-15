#include "player.h"

#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int PLAYER_WIDTH = 58; 
PRIVATE int PLAYER_HEIGHT = 64; 

PRIVATE void initSpriteClips(Player player);

PUBLIC Player createPlayer(int x, int y, int frame, Game game) {
    Player player = malloc(sizeof(struct PlayerSettings));

    initSpriteClips(player);

    player->position.x = x;
    player->position.y = y;
    player->position.w = PLAYER_WIDTH;
    player->position.h = PLAYER_HEIGHT;
    player->frame = frame;
    player->surface = IMG_Load("resources/player.png");         // Temporary sprite
    player->texture = SDL_CreateTextureFromSurface(game->renderer, player->surface);
    SDL_FreeSurface(player->surface);

    return player;
}

PRIVATE void initSpriteClips(Player player) {
    player->spriteClips[0].x = 0;
    player->spriteClips[0].y = 0;
    player->spriteClips[0].w = 58;
    player->spriteClips[0].h = 64;
    
    player->spriteClips[1].x = 58;
    player->spriteClips[1].y = 0;
    player->spriteClips[1].w = 58;
    player->spriteClips[1].h = 64;

    for (int i = 2; i < 16; i++) {          // Temporary values
        player->spriteClips[i].x = 0;
        player->spriteClips[i].y = 0;
        player->spriteClips[i].w = 58;
        player->spriteClips[i].h = 64;
    }
}
