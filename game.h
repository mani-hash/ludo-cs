#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "types.h"

// Function declaration header files

// Game initialization functions
struct Piece createPiece(char namePrefix, char nameSuffix);
struct Player createPlayer(int start, char namePrefix, enum Color color);
struct Player* initializePlayers();
void initializePlayerOrder(struct Game *game, int maxPlayerIndex);

#endif // !GAME