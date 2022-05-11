#include "player.h"

#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int PLAYER_WIDTH = 64;
PRIVATE int PLAYER_HEIGHT = 64;

PRIVATE void initClips(Player player);
PUBLIC void renderPlayers(Game game);

PUBLIC Player createPlayer(int playerNumber, int x, int y, Game game) {
    Player player = malloc(sizeof(struct PlayerSettings));

    char pictureDestination[64];

    switch (playerNumber) {                                                 // Laddar upp rätt bild för rätt player
    case 1:
        SDL_strlcpy(pictureDestination, "resources/old-man.png", sizeof pictureDestination);
        loadTextures(&game->renderer, &game->bitmapSurface, &player->texture, pictureDestination);
        player->currentFrame = 8;                                   // Player is turned right from start
        break;

    case 2:
        SDL_strlcpy(pictureDestination, "resources/green-man.png", sizeof pictureDestination);
        loadTextures(&game->renderer, &game->bitmapSurface, &player->texture, pictureDestination);
        player->currentFrame = 12;                                  // Player is turned left from start
        break;

    case 3:
        SDL_strlcpy(pictureDestination, "resources/blue-man.png", sizeof pictureDestination);
        loadTextures(&game->renderer, &game->bitmapSurface, &player->texture, pictureDestination);
        player->currentFrame = 8;
        break;

    case 4:
        SDL_strlcpy(pictureDestination, "resources/red-man.png", sizeof pictureDestination);
        loadTextures(&game->renderer, &game->bitmapSurface, &player->texture, pictureDestination);
        player->currentFrame = 12;
        break;
    }

    player->pos.x = x;
    player->pos.y = y;
    player->pos.w = PLAYER_WIDTH;
    player->pos.h = PLAYER_HEIGHT;
    player->hitboxPos.w = 34;            // The actual width of the character in each clip
    player->hitboxPos.h = 46;            // The actual height of the character in each clip
    player->speed = 2;
    player->bombsAvailable = 1;
    player->explosionRange = 2;
    // player->numOfBoxes = 1;
    player->isHurt = false;
    player->isAlive = true;
    player->lifes = 3;
    player->prevPosX = 0; 
    player->prevPosY = 0; 
    player->maxBombs = 1; 
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
    if (!game->player[game->pIdx]->isAlive) {
        SDL_RenderCopy(game->renderer, game->dead, NULL, NULL);
    }
    
    for (uint8_t i = 0; i < game->activePlayers; i++) {
        if (game->player[i]->isAlive) {
            if (game->player[i]->isHurt)
                SDL_SetTextureColorMod(game->player[i]->texture, 255, 0, 0);           // Character turns red if hurt
            else {
                SDL_SetTextureColorMod(game->player[i]->texture, 255, 255, 255);       // Restore color if not hurt
            }

            SDL_RenderCopy(game->renderer, game->player[i]->texture, &game->player[i]->clip[game->player[i]->currentFrame], &game->player[i]->pos);
        }
    }
}