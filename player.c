#include "player.h"

#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int PLAYER_WIDTH = 58;
PRIVATE int PLAYER_HEIGHT = 64;

struct PlayerSettings {
    int pos_x; 
    int pos_y;
    int frame;
    int speed; 
};


PUBLIC Player createPlayer(int x, int y, Game game) {
    Player player = malloc(sizeof(struct PlayerSettings));
    player->pos_x = x;
    player->pos_y = y;
    return player;
}