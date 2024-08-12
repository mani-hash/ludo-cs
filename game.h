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

// helper methods
enum Color getPieceColor(char colorLetter);
char* getName(enum Color color);
int getStartIndex(enum Color color);
int getApproachIndex(enum Color color);
int getNoOfPiecesInBase(struct Player *player);
bool canMoveToBoard(int diceNumber);
int getEnemyCountOfCell(struct Piece *cells[PLAYER_NO], enum Color playerColor);
bool isBlocked(int playerPieceCount, int enemyPieceCount);
bool isCellEmpty(struct Piece *cells[PLAYER_NO]);

#endif // !GAME