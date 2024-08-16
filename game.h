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
int getPlayerCountOfCell(struct Piece *cells[PLAYER_NO], enum Color playerColor);
int getEnemyCountOfCell(struct Piece *cells[PLAYER_NO], enum Color playerColor);
bool isPlayerCell(struct Piece *cells[PLAYER_NO], enum Color playerColor);
bool isBlocked(int playerPieceCount, int enemyPieceCount);
bool isCellEmpty(struct Piece *cells[PLAYER_NO]);
int getMysteryLocation(int mysteryEffect, struct Piece *piece);
bool boardHasPiece(struct Player *players);
int getCorrectCellCount(int cellCount);

// game methods/actions
int rollDice();
bool getDirectionFromToss();
int getMysteryEffect();
void moveFromBase(struct Player *player, struct Piece *piece, struct Piece *cell[PLAYER_NO]);
void allocateMysteryCell(struct Game *game, struct Piece *pieces[][PIECE_NO]);
void applyMysteryEffect(int mysteryEffect, int mysteryLocation, struct Piece *piece);
void applyTeleportation(struct Piece *pieces[], int count, struct Piece *cells[][PLAYER_NO]);

// Behavior functions
void redMoveParse(struct Player *player, int redPlayerIndex, int diceNumber, struct Piece *cells[][PIECE_NO]);
bool initialRedMovementCheck
(
  struct Player *player,
  struct RedPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber 
);
void validateSingleRedMovement
(
  struct Player *player,
  struct RedPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber
);
bool validateSingleRedTargetCell
(
  struct Piece *cell[PIECE_NO],
  struct RedPriority *piecePriorities,
  bool partialMoveCondition,
  int pieceIndex,
  enum Color color
);
void validateBlockRedMovement(
  struct Player *player,
  struct RedPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int playerCount,
  int pieceIndex,
  int diceNumber
);
bool validateSingleBlockTargetCell(
  struct Piece *cell[PIECE_NO],
  struct RedPriority *piecePriorities,
  bool partialMoveCondition,
  int pieceIndex,
  int playerCount,
  enum Color color
);

// Output functions
void displayPlayerStatusAfterRound(struct Player *players, struct Game *game);


// game loops
void initialGameLoop(struct Player *players, struct Game *game);
void mainGameLoop(struct Player *players, struct Game *game, struct Piece *standardCells[][PLAYER_NO], struct Piece *homeStraight[][MAX_HOME_STRAIGHT/PLAYER_NO]);

#endif // !GAME