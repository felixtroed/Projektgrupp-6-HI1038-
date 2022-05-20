#include "player.h"

#define PUBLIC /* empty */
#define PRIVATE static

PRIVATE int PLAYER_WIDTH = 64;
PRIVATE int PLAYER_HEIGHT = 64;
PRIVATE void initClips(Player player);

PUBLIC int getPlayerFrame(Player player) {
    return player->currentFrame;
}

PUBLIC void incrementPlayerFrame(Player player) {
    (player->currentFrame)++;
}

PUBLIC void decreasePlayerPosX(Player player) {
    player->pos.x -= player->speed;
}

PUBLIC void decreasePlayerPosY(Player player) {
    player->pos.y -= player->speed;
}

PUBLIC void increasePlayerPosX(Player player) {
    player->pos.x += player->speed;
}

PUBLIC void increasePlayerPosY(Player player) {
    player->pos.y += player->speed;
}

PUBLIC void incrementPlayerExpRange(Player player) {
    (player->explosionRange)++;
}

PUBLIC void incrementMaxBombs(Player player) {
    (player->maxBombs)++;
}

PUBLIC int getMaxBombs(Player player) {
    return player->maxBombs;
}

PUBLIC void destroyPlayerTexture(Player player) {
    SDL_DestroyTexture(player->texture);
}

PUBLIC void setPlayerSpeed(Player player, int speed) {
    player->speed = speed;
}

PUBLIC int getPlayerSpeed(Player player) {
    return player->speed;
}

PUBLIC void incrementPlayerSpeed(Player player) {
    (player->speed)++;
}

PUBLIC void setBombsAvailable(Player player, uint8_t bombs) {
    player->bombsAvailable = bombs;
}

PUBLIC void incrementBombsAvailable(Player player) {
    (player->bombsAvailable)++;
}

PUBLIC void setPlayerLives(Player player, uint8_t lives) {
    player->lifes = lives;
}

PUBLIC bool playerIsLastPlayer(Player player) {
    if (player->lastPlayer) {
        return true;
    }
    else return false;
}

PUBLIC bool playerHasLivesRemaining(Player player) {
    if (player->lifes <= 0) {
        return false;
    }
    else return true;
}

PUBLIC void decrementPlayerLives(Player player) {
    (player->lifes)--;
}

PUBLIC SDL_Rect* getPlayerHitboxPos(Player player) {
    return &(player->hitboxPos);
}

PUBLIC void modifyHitboxPos(Player player) {
    player->hitboxPos.x = getPlayerPosX(player) + 16;       // Puts hitbox in the same position as character
    player->hitboxPos.y = getPlayerPosY(player) + 14;
}

PUBLIC bool playerIsHurt(Player player) {
    if (player->isHurt) {
        return true; 
    }
    else return false;
}

PUBLIC void setToLastPlayer(Player player) {
    player->lastPlayer = true;
}

PUBLIC void setToNotLastPlayer(Player player) {
    player->lastPlayer = false;
}

PUBLIC void setPlayerToDead(Player player) {
    player->isAlive = false;
}

PUBLIC void setPlayerToAlive(Player player) {
    player->isAlive = true;
}

PUBLIC void setPlayerToHurt(Player player) {
    player->isHurt = true;
}

PUBLIC void setPlayerToNotHurt(Player player) {
    player->isHurt = false;
}

PUBLIC void setPlayerExpRange(Player player, int explosionRange) {
    player->explosionRange = explosionRange;
}

PUBLIC void setPlayerFrame(Player player, int frame) {
    player->currentFrame = frame;
}

PUBLIC void setPlayerPosY(Player player, int y) {
    player->pos.y = y;
}

PUBLIC void setPlayerPosX(Player player, int x) {
    player->pos.x = x;
}

PUBLIC uint8_t* getBombsAvailableVal(Player player) {
    return &(player->bombsAvailable);
}

PUBLIC int getPlayerExpRange(Player player) {
    return player->explosionRange;
}

PUBLIC int getPlayerPosX(Player player) {
    return player->pos.x;
}

PUBLIC int getPlayerPosY(Player player) {
    return player->pos.y;
}

PUBLIC void decrementBombsAvailable(Player player) {
    (player->bombsAvailable)--;
}

PUBLIC bool playerIsAlive(Player player) {
    if (player->isAlive) {
        return true; 
    }
    else return false; 
}

PUBLIC bool bombsAvailable(Player player) {
    if (player->bombsAvailable) {
        return true; 
    }
    else return false;
}

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
    player->isHurt = false;
    player->isAlive = true;
    player->lastPlayer = false;
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