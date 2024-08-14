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
        0,
        0,
        {[0 ... PLAYER_NO - 1] = -1},
        {[0 ... PLAYER_NO - 1] = -1},
        -1,

    };

    struct Piece *standardCells[MAX_STANDARD_CELL][PIECE_NO] = {NULL};

    struct Piece *homeStraight[PLAYER_NO][MAX_HOME_STRAIGHT / PIECE_NO] = {NULL};

    struct Player *players = initializePlayers();

    for (int playerIndex = 0; playerIndex < PLAYER_NO; playerIndex++)
    {
        printf
        (
            "The %s player has %d pieces %s, %s, %s, and %s\n",
            getName(players[playerIndex].color),
            PIECE_NO,
            players[playerIndex].pieces[0].name,
            players[playerIndex].pieces[1].name,
            players[playerIndex].pieces[2].name,
            players[playerIndex].pieces[3].name
        );
    }
    
    printf("\n");

    // Seed the random number generator
    srand(time(NULL));

    initialGameLoop(players, &game);
    
    mainGameLoop(players, &game, standardCells, homeStraight);

    free(players);
}