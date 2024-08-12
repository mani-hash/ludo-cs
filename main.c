#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "types.h"

void playGame();

int main()
{
    playGame();
    return 0;
}

void playGame()
{
    struct Game game = {
        0,
        -1,
        -1,
        {[0 ... PLAYER_NO - 1] = -1},
        {[0 ... PLAYER_NO - 1] = -1},
        {[0 ... MAX_STANDARD_CELL - 1] = -1},

    };

    struct Piece *standardCells[MAX_STANDARD_CELL][PIECE_NO] = {NULL};

    struct Piece *homeStraight[PLAYER_NO][MAX_HOME_STRAIGHT / PIECE_NO] = {NULL};

    struct Player* players = initializePlayers();

    // Seed the random number generator
    srand(time(NULL));

    free(players);
}