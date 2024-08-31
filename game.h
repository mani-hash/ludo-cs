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

// error functions
void tryValueAndCatchError(int targetValue, char comparison, int compareValue);
void displayErrors();

// helper methods
bool cellNoIndexable(int cellNo);
enum Color getPieceColor(char colorLetter);
char* getName(enum Color color);
int getStartIndex(enum Color color);
int getApproachIndex(enum Color color);
int getNoOfPiecesInBase(struct Player *player);
bool canMoveToBoard(int diceNumber);
int getPlayerCountOfCell(struct Piece *cells[PLAYER_NO], enum Color playerColor);
int getEnemyCountOfCell(struct Piece *cell[PLAYER_NO], enum Color playerColor);
bool isPlayerCell(struct Piece *cells[PLAYER_NO], enum Color playerColor);
bool isBlockade(struct Piece *cell[PIECE_NO]);
bool isBlocked(int playerPieceCount, int enemyPieceCount);
bool isCellEmpty(struct Piece *cells[PLAYER_NO]);
int getMysteryEffectNumber(int location);
int getMysteryLocation(int mysteryEffect, struct Piece *piece);
char *getMysteryLocationName(int mysteryEffect);
bool boardHasPiece(struct Player *players);
int getCorrectCellCount(int cellCount);
int getMovableCellCount
(
  int cellNo,
  int diceNumber,
  bool clockWise,
  int playerCount,
  struct Piece *cells[][PIECE_NO],
  enum Color color
);
int checkIfCellIsPassable(struct Piece *cell[PIECE_NO], enum Color color, int playerCount);
enum Color getPlayerColorInCell(struct Piece *cell[PIECE_NO]);
int getCaptureCountOfPlayer(struct Player *player);
int getDistanceFromHome(struct Piece *piece);
int getEnemyDistanceFromHome(struct Piece *cell[PIECE_NO]);
bool playerHasBlock(struct Player *player);
int getCellNoOfRandomBlock(struct Player *player, struct Piece *cells[][PIECE_NO]);
bool pieceInApproachRange(struct Piece *piece, struct Piece *cells[][PIECE_NO], int finalCellNo);
bool canEnterHomeStraight(struct Piece *piece);
bool canMoveToHome(int cellNo, int diceNumber);
bool canMoveInHomeStraight(int cellNo, int diceNumber);

// game methods/actions
int rollDice();
bool getDirectionFromToss();
int getMysteryEffect();
void formBlock(struct Piece *cell[PIECE_NO]);
void moveFromBase(struct Player *player, struct Piece *piece, struct Piece *cell[PLAYER_NO]);
void allocateMysteryCell(struct Game *game, struct Piece *pieces[][PIECE_NO]);
void applyMysteryEffect(int mysteryEffect, int mysteryLocation, struct Piece *piece, char *playerName, char *pieceName);
void applyTeleportation(struct Piece **pieces, int mysteryEffect, struct Piece *cells[][PLAYER_NO]);
int getDiceValueAfterMysteryEffect(int diceNumber, struct Player *player, int pieceIndex);
void resetPiece(struct Piece *piece);
void decrementMysteryEffectRounds(struct Piece *pieces);
void resetMysteryEffect(struct Piece *pieces);
void captureByPiece(struct Piece *piece, struct Piece *cells[][PIECE_NO], int finalCellNo, char *playerName);
void captureByBlock
(
  struct Piece **blockPieces,
  int playerCount,
  struct Piece *cells[][PIECE_NO],
  int finalCellNo,
  char *playerName
);
void separateBlockade(struct Piece *cells[][PIECE_NO], int blockCellNo);
void move(struct Piece *piece, int prevIndex, int diceNumber, struct Piece *cells[][PIECE_NO]);
void moveBlock(struct Piece *piece, int diceNumber, struct Piece *cells[][PIECE_NO]);
void moveInHomeStraight(struct Piece *piece, int diceNumber);
void handlePieceLandOnMysteryCell(struct Game *game, struct Player *player, struct Piece *cells[][PIECE_NO]);
bool handleCellToHomeStraight(struct Piece *piece, int diceNumber, int movableCellCount, int finalCellNo, struct Piece *cells[][PIECE_NO]);
void incrementHomeApproachPasses(struct Piece *piece, struct Piece *cells[][PIECE_NO], int finalCellNo);

// Behavior functions
void moveParse(struct Player *players, int playerIndex, int diceNumber, struct Piece *cells[][PIECE_NO], int curMyseryCell);
bool initialMovementCheck
(
  struct Player *player,
  union PiecePriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber 
);
void validateSingleMovement
(
  struct Player *player,
  union PiecePriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber,
  int curMysteryCell
);
void validateBlockMovement
(
  struct Player *player,
  union PiecePriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber,
  int curMysteryCell
);
int getIndexOfSelectedPiece
(
  struct Piece *pieces,
  struct Piece *cells[][PIECE_NO],
  int *pieceImportance,
  int canAttackCount,
  int diceNumber
);
void finalizeMovement
(
  struct Player *player,
  int selectedPieceIndex, 
  int diceNumber, 
  struct Piece *cells[][PIECE_NO], 
  bool blockMoveCondition
);

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
void validateBlockRedMovement(
  struct Player *player,
  struct RedPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber
);
void validateRedPieceImportance(
  struct RedPriority *piecePriorities,
  int *pieceImportance,
  int pieceIndex,
  int *canAttackCount,
  bool isPartofBlockade
);
int getIndexOfSelectedRedPiece
(
  struct Piece *pieces,
  struct Piece *cells[][PIECE_NO],
  int *pieceImportance,
  int canAttackCount,
  int diceNumber
);
void finalizeRedMovement
(
  struct Player *player,
  int selectedPiece, 
  int diceNumber, 
  struct Piece *cells[][PIECE_NO], 
  bool canExitBlock
);

void greenMoveParse(struct Player *players, int greenPlayerIndex, int diceNumber, struct Piece *cells[][PIECE_NO]);
bool initialGreenMovementCheck
(
  struct Player *player,
  struct GreenPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber 
);
void validateSingleGreenMovement
(
  struct Player *player,
  struct GreenPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber
);
void validateBlockGreenMovement
(
  struct Player *player,
  struct GreenPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber
);
void validateGreenPieceImportance
(
  struct GreenPriority *piecePriorities,
  int *pieceImportance,
  int pieceIndex
);
int getIndexOfSelectedGreenPiece
(
  struct Piece *pieces,
  struct Piece *cells[][PIECE_NO],
  int *pieceImportance,
  int diceNumber
);
void finalizeGreenMovement
(
  struct Player *player,
  int selectedPieceIndex, 
  int diceNumber, 
  struct Piece *cells[][PIECE_NO],
  bool canMoveBlock
);

void yellowMoveParse(struct Player *players, int yellowPlayerIndex, int diceNumber, struct Piece *cells[][PIECE_NO]);
bool initialYellowMovementCheck
(
  struct Player *player,
  struct YellowPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber 
);
void validateSingleYellowMovement
(
  struct Player *player,
  struct YellowPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber
);
void validateBlockYellowMovement
(
  struct Player *player,
  struct YellowPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber
);
void validateYellowPieceImportance
(
  struct YellowPriority *piecePriorities,
  int *pieceImportance,
  int pieceIndex
);
int getIndexOfSelectedYellowPiece
(
  struct Piece *pieces,
  struct Piece *cells[][PIECE_NO],
  int *pieceImportance,
  int diceNumber
);
void finalizeYellowMovement
(
  struct Player *player,
  int selectedPieceIndex, 
  int diceNumber, 
  struct Piece *cells[][PIECE_NO], 
  bool canExitBlock
);

void blueMoveParse(struct Player *players, int yellowPlayerIndex, int diceNumber, int curMysteryCell, struct Piece *cells[][PIECE_NO]);
bool initialBlueMovementCheck
(
  struct Player *player,
  struct BluePriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber 
);
void validateSingleBlueMovement
(
  struct Player *player,
  struct BluePriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber,
  int curMysteryCell
);
void validateBlockBlueMovement
(
  struct Player *player,
  struct BluePriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber,
  int curMysteryCell
);
void validateBluePieceImportance
(
  struct BluePriority *piecePriorities,
  int *pieceImportance,
  int pieceIndex,
  int previousPieceIndex
);
int getIndexOfSelectedBluePiece
(
  struct Piece *pieces,
  struct Piece *cells[][PIECE_NO],
  int *pieceImportance,
  int diceNumber
);
void finalizeBlueMovement
(
  struct Player *player,
  int selectedPieceIndex, 
  int diceNumber, 
  struct Piece *cells[][PIECE_NO], 
  bool canExitBlock
);

// Output functions
void displayPlayerStatusAfterRound(struct Player *players, struct Game *game);
void displayMysteryCellStatusAfterRound(int mysteryCellNo, int mysteryRounds);
void displayTeleportationMessage(char* playerName, int count, struct Piece **pieces, char *location);
void displayMovablePieceStatus
(
  int movableCellCount,
  int diceNumber,
  char *playerName,
  struct Piece *piece,
  int finalCellNo,
  struct Piece *cell[PIECE_NO]
);
void displayMovableBlockStatus(
  int movableCellCount,
  int diceNumber,
  char *playerName,
  struct Piece *piece,
  int finalCellNo,
  struct Piece *cell[PIECE_NO]
);

// game loops
void initialGameLoop(struct Player *players, struct Game *game);
void handleMysteryCellLoop(struct Game *game, struct Player *players, struct Piece *cells[][PIECE_NO]);
void mainGameLoop(struct Player *players, struct Game *game, struct Piece *standardCells[][PLAYER_NO]);

#endif // !GAME