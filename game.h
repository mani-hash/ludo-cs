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

// game methods/actions
int rollDice();
bool getDirectionFromToss();
int getMysteryEffect();
void moveFromBase(struct Player *player, struct Piece *piece, struct Piece *cell[PLAYER_NO]);
void allocateMysteryCell(struct Game *game, struct Piece *pieces[][PLAYER_NO]);

// Output functions
void displayPlayerStatusAfterRound(struct Player *players, struct Game *game);


// game loops
void initialGameLoop(struct Player *players, struct Game *game);
void mainGameLoop(struct Player *players, struct Game *game, struct Piece *standardCells[][PLAYER_NO], struct Piece *homeStraight[][MAX_HOME_STRAIGHT/PLAYER_NO]);

#endif // !GAME