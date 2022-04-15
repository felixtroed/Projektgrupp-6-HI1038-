#include "player.h"

#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int PLAYER_WIDTH = 58; 
PRIVATE int PLAYER_HEIGHT = 64; 

struct PlayerSettings {
    SDL_Rect position;
    int frame;
};

PUBLIC Player createPlayer(int x, int y, int frame) {
    Player player = malloc(sizeof(struct PlayerSettings));
    player->position.x = x;
    player->position.y = y;
    player->position.w = PLAYER_WIDTH;
    player->position.h = PLAYER_HEIGHT;
    player->frame = frame;
    return player;
}
