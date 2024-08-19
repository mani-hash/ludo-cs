#include "game.h"
#include "types.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* Initialization functions
 */

struct Piece createPiece(char namePrefix, char nameSuffix)
{
  char name[3] = {namePrefix, nameSuffix, '\0'};

  struct Piece piece;

  piece.cellNo = BASE;
  piece.captured = 0;
  strncpy(piece.name, name, sizeof(piece.name) - 1);
  piece.name[sizeof(piece.name) - 1] = '\0'; //explicit null termination
  piece.clockWise = true;
  piece.block = false;
  piece.blockClockWise = true;
  piece.noOfApproachPasses = 0;

  //define default effect status
  piece.effect.effectActive = false;
  piece.effect.pieceActive = true;
  piece.effect.effectActiveRounds = 0;
  piece.effect.diceMultiplier = 1;
  piece.effect.diceDivider = 1;

  return piece;
}

struct Player createPlayer(int start, char namePrefix, enum Color color)
{
  struct Player player = {
    start,
    {
      createPiece(namePrefix, '1'),
      createPiece(namePrefix, '2'),
      createPiece(namePrefix, '3'),
      createPiece(namePrefix, '4')
    },
    color
  };

  return player;
}

struct Player* initializePlayers()
{
  struct Player *players = (struct Player*)malloc(PLAYER_NO * sizeof(struct Player));

  players[0] = createPlayer(YELLOW_START, 'Y', YELLOW);

  players[1] = createPlayer(BLUE_START, 'B', BLUE);

  players[2] = createPlayer(RED_START, 'R', RED);

  players[3] = createPlayer(GREEN_START, 'G', GREEN);

  return players;
}

void initializePlayerOrder(struct Game *game, int maxPlayerIndex)
{
  //calculate difference to offset 
  //order array index to initial index 
  //(prevent array out of bound error)
  int difference;

  switch (maxPlayerIndex)
  {
    case 1:
      difference = 3;
      break;
    case 2:
      difference = 2;
      break;
    case 3:
      difference = 1;
      break;
    default:
      difference = 0;
  }

  game->order[0] = maxPlayerIndex;
  for (int order = 1; order < PLAYER_NO; order++)
  {
    game->order[order] = (maxPlayerIndex+order >= PLAYER_NO) ? order - difference : maxPlayerIndex+order;
  }
}

/* Helper methods
 */

enum Color getPieceColor(char colorLetter)
{
  switch (colorLetter)
  {
    case 'R':
      return RED;
    case 'G':
      return GREEN;
    case 'Y':
      return YELLOW;
    case 'B':
      return BLUE;
  }
}

// Get name of the current player
char* getName(enum Color color)
{
  switch (color)
  {
    case RED:
      return "Red";
    case GREEN:
      return "Green";
    case YELLOW:
      return "Yellow";
    case BLUE:
      return "Blue";
  }
}

int getStartIndex(enum Color color)
{
  switch (color)
  {
    case RED:
      return RED_START;
    case GREEN:
      return GREEN_START;
    case YELLOW:
      return YELLOW_START;
    case BLUE:
      return BLUE_START;
  }
}

int getApproachIndex(enum Color color)
{
  switch (color)
  {
    case RED:
      return RED_APPROACH;
    case GREEN:
      return GREEN_APPROACH;
    case YELLOW:
      return YELLOW_APPROACH;
    case BLUE:
      return BLUE_APPROACH;
  }
}

int getNoOfPiecesInBase(struct Player *player)
{
  int count = 0;
  for (int pieceIndex = 0; pieceIndex < PLAYER_NO; pieceIndex++)
  {
    if (player->pieces[pieceIndex].cellNo == BASE)
    {
      count++;
    }
  }

  return count;
}

bool canMoveToBoard(int diceNumber)
{
  if (diceNumber == 6)
  {
    return true;
  }

  return false;
}

int getPlayerCountOfCell(struct Piece *cells[PLAYER_NO], enum Color playerColor)
{
  int count = 0;
  for (int cellIndex = 0; cellIndex < PLAYER_NO; cellIndex++)
  {
    if (
      cells[cellIndex] != NULL 
      &&
      getPieceColor(cells[cellIndex]->name[0]) == playerColor
    )
    {
      count++;
    }
  }

  return count;
}

int getEnemyCountOfCell(struct Piece *cells[PLAYER_NO], enum Color playerColor)
{
  int count = 0;
  for (int cellIndex = 0; cellIndex < PLAYER_NO; cellIndex++)
  {
    if (
      cells[cellIndex] != NULL 
      &&
      getPieceColor(cells[cellIndex]->name[0]) != playerColor
    )
    {
      count++;
    }
  }

  return count;
}

bool isPlayerCell(struct Piece *cells[PLAYER_NO], enum Color playerColor)
{
  for (int cellIndex = 0; cellIndex < PLAYER_NO; cellIndex++)
  {
    if (cells[cellIndex] != NULL && getPieceColor(cells[cellIndex]->name[0]) == playerColor)
    {
      return true;
    }
  }
  return false;
}

bool isBlocked(int playerPieceCount, int enemyPieceCount)
{
  if (enemyPieceCount > playerPieceCount)
  {
    return true;
  }
  return false;
}

// Check if piece is occupied in the current cell
bool isCellEmpty(struct Piece *cells[PLAYER_NO])
{
  for (int cellIndex = 0; cellIndex < PLAYER_NO; cellIndex++)
  {
    if (cells[cellIndex] != NULL)
    {
      return false;
    }
  }

  return true;
}

int getMysteryLocation(int mysteryEffect, struct Piece *piece)
{
  switch (mysteryEffect)
  {
    case 1:
      return BHAWANA;
    case 2:
      return KOTUWA;
    case 3:
      return PITA_KOTUWA;
    case 4:
      return BASE;
    case 5:
      return getStartIndex(piece->name[0]);
    case 6:
      return getApproachIndex(piece->name[0]);
  }
}

bool boardHasPiece(struct Player *players)
{
  for (int playerIndex = 0; playerIndex < PLAYER_NO; playerIndex++)
  {
    for (int pieceIndex = 0; pieceIndex < PIECE_NO; pieceIndex++)
    {
      int cellNo = players[playerIndex].pieces[pieceIndex].cellNo;
      if (cellNo != HOME && cellNo != BASE)
      {
        return true;
      }
    }
  }

  return false;
}

int getCorrectCellCount(int cellCount)
{
  if (cellCount < 0)
  {
    return MAX_STANDARD_CELL + cellCount;
  }
  else if (cellCount >= MAX_STANDARD_CELL)
  {
    return cellCount - MAX_STANDARD_CELL;
  }

  return cellCount;
}

int getMovableCellCount
(
  int cellNo,
  int diceNumber,
  bool clockWise,
  int playerCount,
  struct Piece *cells[][PIECE_NO],
  enum Color color
)
{
  int movableCellCount = 0;

  if (clockWise)
  {
    int maxCellNo = cellNo + diceNumber;
    for (int cellIndex = cellNo + 1; cellIndex <= maxCellNo; cellIndex++)
    {
      int correctCellIndex = getCorrectCellCount(cellIndex);
      
      movableCellCount += checkIfCellIsPassable(cells[correctCellIndex], color, playerCount);
    } 
  }
  else
  {
    int maxCellNo = cellNo - diceNumber;
    for (int cellIndex = cellNo - 1; cellIndex >= maxCellNo; cellIndex--)
    {
      int correctCellIndex = getCorrectCellCount(cellIndex);

      movableCellCount += checkIfCellIsPassable(cells[correctCellIndex], color, playerCount);
    }
  }

  return movableCellCount;
  
}

int checkIfCellIsPassable(struct Piece *cell[PIECE_NO], enum Color color, int playerCount)
{
  int enemyCountOfCell = getEnemyCountOfCell(cell, color);

  if (isCellEmpty(cell))
  {
    return 1;
  }
  else if (!isBlocked(playerCount, enemyCountOfCell))
  {
    return 1;
  }

  return 0;
}

enum Color getPlayerColorInCell(struct Piece *cell[PIECE_NO])
{
  enum Color color;

  for (int cellIndex = 0; cellIndex < PIECE_NO; cellIndex++)
  {
    if (cell[cellIndex] != NULL)
    {
      color = getPieceColor(cell[cellIndex]->name[0]);
    }
  }

  return color;
}

int getCaptureCountOfPlayer(struct Player *player)
{
  int captured = 0;
  for (int pieceIndex = 0; pieceIndex < PIECE_NO; pieceIndex++)
  {
    captured += player->pieces[pieceIndex].captured;
  }

  return captured;
}

/* Game methods/actions
 */

int rollDice()
{
  int diceNumber = (rand() % 6) + 1;
  return diceNumber;
}

// Coin toss
bool getDirectionFromToss()
{
  bool clockWise = rand() % 2;

  return clockWise;
}

int getMysteryEffect()
{
  int mysteryEffect = (rand() % MYSTERY_LOCATIONS) + 1;
  return mysteryEffect;
}

void moveFromBase(struct Player *player, struct Piece *piece, struct Piece *cell[PLAYER_NO])
{
  enum Color color = getPieceColor(piece->name[0]);
  char *pieceColor = getName(color);
  int enemyCount = getEnemyCountOfCell(cell, color);

  if (!isBlocked(1, enemyCount))
  {
    int cellIndex;
    for (cellIndex = 0; cellIndex < PLAYER_NO; cellIndex++)
    {
      if (cell[cellIndex] != NULL && cell[cellIndex]->name[0] != piece->name[0])
      {
        cell[cellIndex]->cellNo = BASE;
        piece->captured += 1;
        break;
      }
    }

    cell[cellIndex == PLAYER_NO ? cellIndex - 1 : cellIndex] = piece;
    piece->cellNo = player->startIndex;
    piece->clockWise = getDirectionFromToss();

    int noOfPiecesInBase = getNoOfPiecesInBase(player);

    printf("%s moves piece %s to the starting point\n", pieceColor, piece->name);
    printf("%s player now has %d/4 of pieces on the board and %d/4 pieces on the base\n\n",
      pieceColor,
      PLAYER_NO - noOfPiecesInBase,
      noOfPiecesInBase
    );
  }
}

void allocateMysteryCell(struct Game *game, struct Piece *pieces[][PIECE_NO])
{
  //very inefficient algorithm try to fix it later
  bool repeat = true;

  do
  {
    int mysteryCell = (rand() % MAX_STANDARD_CELL - 1) + 1;

    if (mysteryCell != game->prevMysteryCell && isCellEmpty(pieces[mysteryCell]))
    {
      repeat = false;
      game->prevMysteryCell = game->mysteryCellNo;
      game->mysteryCellNo = mysteryCell;
    }
  }
  while (repeat);
}

void applyMysteryEffect(int mysteryEffect, int mysteryLocation, struct Piece *piece)
{
  switch (mysteryEffect)
  {
    case 1: // bhawana
      int energy = rand() % 2;
      piece->effect.effectActive = true;
      piece->effect.effectActiveRounds = 4;

      if (energy)
      {
        piece->effect.diceMultiplier = 2;  
      }
      else
      {
        piece->effect.diceDivider = 2;
      }

      break;
    case 2: // kotuwa
      piece->effect.effectActive = true;
      piece->effect.pieceActive = false;
      piece->effect.effectActiveRounds = 4;
      break;
    case 3: // pita kotuwa
      if (piece->clockWise)
      {
        piece->clockWise = false;
      }
      else
      {
        piece->effect.effectActive = true;
        piece->effect.pieceActive = false;
        piece->effect.effectActiveRounds = 4; 
      }
      break;
  }

  piece->cellNo = mysteryLocation;
}

void applyTeleportation(struct Piece *pieces[], int count, struct Piece *cells[][PLAYER_NO])
{
  int mysteryEffect = getMysteryEffect();
  int mysteryLocation = getMysteryLocation(mysteryEffect, pieces[0]);
  enum Color color = getPieceColor(pieces[0]->name[0]);
  int enemyCount = getEnemyCountOfCell(cells[mysteryLocation], color);

  if (!isBlocked(count, enemyCount))
  {
    // the whole cell logic is wrong fix it 
    bool captured = false;
    for (int cellIndex = 0; cellIndex < PLAYER_NO; cellIndex++)
    {
      // turn this if condition check into a function
      if (cells[mysteryLocation][cellIndex] != NULL && cells[mysteryLocation][cellIndex]->name[0] != pieces[0]->name[0])
      {
        captured = true;
        cells[mysteryLocation][cellIndex]->cellNo = BASE;
      }
    }

    for (int pieceIndex = 0; pieceIndex < count; pieceIndex++)
    {
      if (captured)
      {
        pieces[pieceIndex]->captured += 1;
      }

      for (int cellIndex = 0; cellIndex < PLAYER_NO; cellIndex++)
      {
        if (&cells[mysteryLocation][cellIndex] != NULL)
        {
          continue;
        }
        else
        {
          cells[mysteryLocation][cellIndex] = pieces[pieceIndex];
          break;
        }
      }
    }
  }
}

void playRound()
{

}

void move(struct Piece *piece, int pieceIndex, int diceNumber, struct Piece *cells[][PIECE_NO])
{
  enum Color color = getPieceColor(piece->name[0]);
  char *playerName = getName(color);
  
  int movableCellCount = 
    getMovableCellCount
    (
      piece->cellNo,
      diceNumber,
      piece->clockWise,
      1,
      cells,
      color
    );

  int directonConstant = piece->clockWise ? 1 : -1;

  int finalCellNo = getCorrectCellCount(piece->cellNo + (diceNumber * directonConstant));

  if (movableCellCount == 0)
  {
    printf("%s piece %s is blocked from L%d to L%d by %s piece\n",
      playerName,
      piece->name,
      piece->cellNo,
      finalCellNo,
      getName(getPlayerColorInCell(cells[finalCellNo]))
    );

    printf("%s does not have other pieces to move instead of blocked piece. ", playerName);
    printf("Ignoring the throw and moving to the next player\n");

    return;
  }

  if (movableCellCount < diceNumber)
  {
    finalCellNo = getCorrectCellCount(piece->cellNo + movableCellCount);

    printf("%s does not have other pieces to move instead of blocked piece. Moved the piece %s to square L%d which is a cell before the block\n",
      playerName,
      piece->name,
      finalCellNo
    );
  }
  else
  {
    printf("%s moves piece %s from location L%d to L%d by %d units in %s direction",
      playerName,
      piece->name,
      piece->cellNo,
      finalCellNo,
      movableCellCount,
      piece->clockWise ? "clock-wise" : "counter clock-wise"
    );
  }


  cells[piece->cellNo][pieceIndex] = NULL;
  piece->cellNo = finalCellNo;
    
  if (!isCellEmpty(cells[finalCellNo]))
  {
    if (getEnemyCountOfCell(cells[finalCellNo], color) != 0)
    {
      piece->captured+=1;
      for (int cellIndex = 0; cellIndex < PIECE_NO; cellIndex++)
      {
        if (cells[finalCellNo][cellIndex] != NULL)
        {
          enum Color enemyColor = getPieceColor(cells[finalCellNo][cellIndex]->name[0]);
          char *enemyName = getName(enemyColor);

          printf("%s piece %s lands on square L%d, captures %s piece %s, and returns it to the base",
            playerName,
            piece->name,
            finalCellNo,
            enemyName,
            cells[finalCellNo][cellIndex]->name
          );
          
          cells[finalCellNo][cellIndex]->cellNo = BASE;
          // add reset piece functionality later
          cells[finalCellNo][cellIndex] = NULL;

          // fix later
          // printf("%s player now has %d/4 pieces on the board and %d/4 pieces on the base\n",
          //   enemyName,
          //   getNoOfPiecesInBase()
          // );
          break;
        }
      }
    }
    else
    {
      // form block logic here
      // printf("%s piece %s has formed a block");
    }
  }

  // place piece in 2d array
  for (int cellIndex = 0; cellIndex < PIECE_NO; cellIndex++)
  {
    if (cells[finalCellNo][cellIndex] == NULL)
    {
      cells[finalCellNo][cellIndex] = piece;
      break;
    }
  }
}

/* Behavior functions
 */

void redMoveParse(struct Player *players, int redPlayerIndex, int diceNumber, struct Piece *cells[][PIECE_NO])
{
  struct Player *player = &players[redPlayerIndex];

  struct RedPriority piecePriorities[PIECE_NO] = 
  {
    [0 ... PIECE_NO - 1] = { false, false, false, false, false, false }
  };

  for (int pieceIndex = 0; pieceIndex < PIECE_NO; pieceIndex++)
  {

    // check mystery effects
    if (player->pieces[pieceIndex].effect.effectActive)
    {
      struct MysteryEffects *effect = &player->pieces[pieceIndex].effect;
      if (!effect->pieceActive)
      {
        diceNumber = 0;
      }

      diceNumber *= effect->diceMultiplier;
      diceNumber /= effect->diceDivider;
    }

    if (!initialRedMovementCheck(player, piecePriorities, cells, pieceIndex, diceNumber))
    {
      continue;
    }

    validateSingleRedMovement(player, piecePriorities, cells, pieceIndex, diceNumber);

    int playerCount = getPlayerCountOfCell(cells[player->pieces[pieceIndex].cellNo], player->color);

    // perform block movement check if possible
    if (player->pieces[pieceIndex].block && diceNumber/playerCount != 0)
    {
      validateBlockRedMovement(player, piecePriorities, cells, playerCount, pieceIndex, diceNumber);
    }

  }

  int pieceImportance[] = { 5, 5, 5, 5 };
  int canAttackCount = 0;

  for (int pieceIndex = 0; pieceIndex < PIECE_NO; pieceIndex++)
  {
    if (!piecePriorities[pieceIndex].canAttack)
    {
      // if can't capture
      pieceImportance[pieceIndex] -= 1;

      if (!piecePriorities[pieceIndex].canMoveFromBase)
      {
        pieceImportance[pieceIndex] -= 1;
      }
      else
      {
        continue;
      }

      if (!piecePriorities[pieceIndex].canFullMove)
      {
        pieceImportance[pieceIndex] -= 1;
      }
      
      if (!piecePriorities[pieceIndex].canPartialMove)
      {
        pieceImportance[pieceIndex] -= 1;
      }

      if (player->pieces[pieceIndex].block && !piecePriorities[pieceIndex].canExitBlock)
      {
        pieceImportance[pieceIndex] -= 1;
      }
    }
    // else
    // {
    //   // no change if piece can attack

    //   // if (player->pieces[pieceIndex].block && !piecePriorities[pieceIndex].canExitBlock)
    //   // { // block capture has reduce priority
    //   //   pieceImportance[pieceIndex] -= 1;
    //   // }
    // }

    if (pieceImportance[pieceIndex] == 5)
    {
      canAttackCount++;
    }
  }
  int selectedPiece = -1;
  int maxPriority = -1;
  int distanceFromHome;
  for (int pieceIndex = 0; pieceIndex < PIECE_NO; pieceIndex++)
  {
    if (pieceImportance[pieceIndex] > maxPriority)
    {
      maxPriority = pieceImportance[pieceIndex];
      selectedPiece = pieceIndex;

    }
    // if (pieceImportance[pieceIndex] == 5 && canAttackCount != 0)
    // {

    // }

  }

  if (player->pieces[selectedPiece].cellNo == BASE && diceNumber == MAX_DICE_VALUE)
  {
    moveFromBase(player, &player->pieces[selectedPiece], cells[getStartIndex(player->color)]);
  }
  else if (player->pieces[selectedPiece].cellNo != BASE && player->pieces[selectedPiece].cellNo != HOME)
  {
    move(&player->pieces[selectedPiece], selectedPiece, diceNumber, cells);
  }


}

bool initialRedMovementCheck
(
  struct Player *player,
  struct RedPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber 
)
{
  int cellNo = player->pieces[pieceIndex].cellNo;
  int directionConstant = (player->pieces[pieceIndex].clockWise) ? 1 : -1;
  int maxCellNo = getCorrectCellCount(cellNo + (diceNumber * directionConstant));
  int playerCount = 1;
  int enemyCountOfCell = getEnemyCountOfCell(cells[maxCellNo], player->color);

  //define blockade variables
  bool isBlockade = player->pieces[pieceIndex].block;
  bool isBlockadeMovable;
  int maxBlockCellNo;
  int enemyCountOfCellBlock;

  // if piece is already at home, no movement can be applied!
  if (cellNo == HOME || diceNumber == 0)
  {
    return false;
  }

  if (cellNo == BASE && !isBlocked(1, getEnemyCountOfCell(cells[getStartIndex(player->color)], player->color)) && canMoveToBoard(diceNumber))
  {
    piecePriorities[pieceIndex].canMoveFromBase = true;
    if (getEnemyCountOfCell(cells[getStartIndex(player->color)], player->color) != 0)
    {
      piecePriorities[pieceIndex].canAttack = true;
    }
    return false;
  }

  if (cellNo >= MAX_STANDARD_CELL)
  {
    // handle home straight case here
    piecePriorities[pieceIndex].canFullMove = true;
    return false;
    // maxCellNo -= (maxCellNo - MAX_STANDARD_CELL);
  }

  // initialize main blockade variables here
  if (isBlockade)
  {
    piecePriorities[pieceIndex].canExitBlock = true;
    playerCount = getPlayerCountOfCell(cells[cellNo], player->color);
    isBlockadeMovable = diceNumber/playerCount != 0;

    // initiate other blockade variables
    if (isBlockadeMovable)
    {
      int blockDirectionConstant = player->pieces[pieceIndex].blockClockWise ? 1 : -1;
      maxBlockCellNo = cellNo + ((diceNumber/playerCount) * blockDirectionConstant);
      maxBlockCellNo = (maxBlockCellNo < 0) ? MAX_STANDARD_CELL + maxBlockCellNo : maxBlockCellNo;
      enemyCountOfCellBlock = getEnemyCountOfCell(cells[maxBlockCellNo], player->color);
    }
  }
  
  if (enemyCountOfCell != 0 && !isBlocked(1, enemyCountOfCell))
  {
    // check if single piece can capture
    piecePriorities[pieceIndex].canAttack = true;
  }
  else if (isBlockade && isBlockadeMovable && enemyCountOfCellBlock != 0 && !isBlocked(playerCount, enemyCountOfCellBlock))
  {
    // check if a block can capture
    piecePriorities[pieceIndex].canAttack = true;
    piecePriorities[pieceIndex].canExitBlock = false;
  }
  else if (isCellEmpty(cells[maxCellNo]))
  {
    // check if a single piece can move to final destination
    piecePriorities[pieceIndex].canFullMove = true;
  }
  else if (isBlockade && isBlockadeMovable && isCellEmpty(cells[maxBlockCellNo]))
  {
    // check if a block can move to final destination
    piecePriorities[pieceIndex].canFullMove = true;
    piecePriorities[pieceIndex].canExitBlock = false;
  }
  else if (enemyCountOfCell == 0)
  {
    // check if a single piece can form a block
    piecePriorities[pieceIndex].canFormBlock = true;
  }

  return true;
}

void validateSingleRedMovement
(
  struct Player *player,
  struct RedPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber
)
{
  int cellNo = player->pieces[pieceIndex].cellNo;
  int playerCount = 1;
  bool isBlockade = player->pieces[pieceIndex].block;

  if (isBlockade)
  {
    int playerCount = getPlayerCountOfCell(cells[cellNo], player->color);
  }

  if (player->pieces[pieceIndex].clockWise)
  {
    int maxCellNo = cellNo + diceNumber;
    for (int cellIndex = cellNo + 1; cellIndex <= maxCellNo; cellIndex++)
    {
      int correctCellNo = getCorrectCellCount(maxCellNo);
      bool partialMoveCondition = cellIndex != cellNo + 1;
      if (!validateSingleRedTargetCell
      (
        cells[correctCellNo], 
        piecePriorities, 
        partialMoveCondition, 
        pieceIndex, 
        player->color
      ))
      {
        break;
      }
    }
  }
  else
  {
    int maxCellNo = cellNo - diceNumber;
    for (int cellIndex = cellNo - 1; cellIndex >= maxCellNo; cellIndex--)
    {
      int correctCellNo = (maxCellNo < 0) ? MAX_STANDARD_CELL + maxCellNo : maxCellNo;
      bool partialMoveCondition = cellIndex != cellNo - 1;

      if (!validateSingleRedTargetCell(
        cells[correctCellNo],
        piecePriorities,
        partialMoveCondition,
        pieceIndex,
        player->color
      ))
      {
        break;
      }
    }
  }
}

bool validateSingleRedTargetCell
(
  struct Piece *cell[PIECE_NO],
  struct RedPriority *piecePriorities,
  bool partialMoveCondition,
  int pieceIndex,
  enum Color color
)
{
  if (!isCellEmpty(cell))
  {
    int enemyCountOfCell = getEnemyCountOfCell(cell, color);
    if (isBlocked(1, enemyCountOfCell))
    {
      piecePriorities[pieceIndex].canAttack = false;
      piecePriorities[pieceIndex].canFullMove = false;

      if (partialMoveCondition)
      {
        piecePriorities[pieceIndex].canPartialMove = true;
      }

      return false;
    }
  }

  return true;
}

void validateBlockRedMovement(
  struct Player *player,
  struct RedPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int playerCount,
  int pieceIndex,
  int diceNumber
)
{
  int cellNo = player->pieces[pieceIndex].cellNo;
  if (player->pieces[pieceIndex].blockClockWise)
  {
    int maxBlockCellNo = cellNo + (diceNumber/playerCount);
    for (int cellIndex = cellNo + 1; cellIndex <= maxBlockCellNo; cellIndex++)
    {
      int correctBlockCellNo = getCorrectCellCount(cellIndex);
      bool partialMoveCondition = cellIndex != cellNo + 1;

      if (!validateSingleBlockTargetCell(
        cells[correctBlockCellNo],
        piecePriorities, 
        partialMoveCondition, 
        pieceIndex,
        playerCount,
        player->color
      ))
      {
        break;
      }
    }
  }
  else
  {
    int maxBlockCellNo = cellNo - (diceNumber/playerCount);
    for (int cellIndex = cellNo - 1; cellIndex >= maxBlockCellNo; cellIndex--)
    {
      int correctBlockCellNo = getCorrectCellCount(cellIndex);
      bool partialMoveCondition = cellIndex != cellNo - 1;

      if (!validateSingleBlockTargetCell(
        cells[correctBlockCellNo],
        piecePriorities, 
        partialMoveCondition, 
        pieceIndex,
        playerCount,
        player->color
      ))
      {
        break;
      }
      
    }
  }
}

bool validateSingleBlockTargetCell(
  struct Piece *cell[PIECE_NO],
  struct RedPriority *piecePriorities,
  bool partialMoveCondition,
  int pieceIndex,
  int playerCount,
  enum Color color
)
{
  int enemyCountOfCell = getEnemyCountOfCell(cell, color);

  if (isBlocked(playerCount, enemyCountOfCell))
  {
    // do this tmrw (also compare differences between isBlocked and is captured)
    piecePriorities[pieceIndex].canFullMove = false;
    piecePriorities[pieceIndex].canAttack = false;

    if (partialMoveCondition)
    {
      piecePriorities[pieceIndex].canPartialMove = true;
    }

    return false;
  }

  return true;
}

void finalizeRedMovement()
{

}

void greenMoveParse()
{
  
}

void yellowMoveParse()
{
  
}

void blueMoveParse()
{
  
}

/* Output Display functions
 */

void displayPlayerStatusAfterRound(struct Player *players, struct Game *game)
{
  printf("Round %d is over. Status of each player is displayed below:\n\n", game->rounds);
  for (int orderIndex = 0; orderIndex < PIECE_NO; orderIndex++)
  {
    int playerIndex = game->order[orderIndex];
    char *playerName = getName(players[playerIndex].color);
    int noOfPiecesInBase = getNoOfPiecesInBase(&players[playerIndex]);

    printf("%s player has %d/4 of pieces on the board and %d/4 pieces on the base\n",
      playerName,
      PLAYER_NO - noOfPiecesInBase,
      noOfPiecesInBase
    );
    printf("=======================================================================\n");
    printf("Location of pieces of %s\n", playerName);
    printf("=======================================================================\n");

    for (int pieceIndex = 0; pieceIndex < PIECE_NO; pieceIndex++)
    {
      struct Piece piece = players[playerIndex].pieces[pieceIndex];

      switch (piece.cellNo)
      {
        case BASE:
          printf("Piece %s -> Base\n", piece.name);
          break;
        case HOME:
          printf("Piece %s -> Home\n", piece.name);
          break;
        default:
          printf("Piece %s -> L%d\n", piece.name, piece.cellNo);
      }
      printf("\n");
    }
  }
}

void displayMysteryCellStatusAfterRound(int mysteryCellNo, int mysteryRounds)
{
  if (mysteryCellNo == -1)
  {
    printf("The required conditions for generating mystery cells have not been met");
  }
  else
  {
    printf("The mystery cell is at L%d and will be at that location for the next %d rounds",
      mysteryCellNo,
      mysteryRounds
    );
  }
}

/* Functions for game loop
 */

void initialGameLoop(struct Player *players, struct Game *game)
{
  int max = -1;
  int maxPlayerIndex = -1;

  // find the player with highest roll
  for (int playerIndex = 0; playerIndex < PLAYER_NO; playerIndex++)
  { 
    int diceNumber = rollDice();
    printf("%s rolls %d\n", getName(players[playerIndex].color), diceNumber);
    if (diceNumber > max)
    {
      max = diceNumber;
      maxPlayerIndex = playerIndex;
    }
  }

  printf("\n%s player has the highest roll and will begin the game\n", getName(players[maxPlayerIndex].color));
  initializePlayerOrder(game, maxPlayerIndex);
  printf("The order of single round is %s, %s, %s, and %s\n\n", 
    getName(players[game->order[0]].color),
    getName(players[game->order[1]].color),
    getName(players[game->order[2]].color),
    getName(players[game->order[3]].color)
  );
}

void handleMysteryCellLoop(struct Game *game, struct Player *players, struct Piece *cells[][PIECE_NO])
{
  if (game->roundsTillMysteryCell <= 2)
  {
    if (boardHasPiece(players))
    {
      game->roundsTillMysteryCell += 1;
    }
  }
  else
  {
    if (game->mysteryRounds == 0)
    {
      allocateMysteryCell(game, cells);
      game->mysteryRounds = 4;
      printf("A mystery cell has spawned in location L%d and will be at this location for the next %d rounds",
        game->mysteryCellNo,
        game->mysteryRounds
      );
    }
    else
    {
      game->mysteryRounds -= 1;
    }
  }
}

void mainGameLoop(struct Player *players, struct Game *game, struct Piece *standardCells[][PLAYER_NO], struct Piece *homeStraight[][MAX_HOME_STRAIGHT/PLAYER_NO])
{
  // test counter
  int test = 0;

  while (true)
  {
    game->rounds += 1;
    printf("=============== Round %d ==============\n\n", game->rounds);

    handleMysteryCellLoop(game, players, standardCells);

    for (int orderIndex = 0; orderIndex < PLAYER_NO; orderIndex++)
    {
      int playerIndex = game->order[orderIndex];
      int diceNumber = rollDice();
      int noOfPiecesInBase = getNoOfPiecesInBase(&players[playerIndex]);

      printf("%s player rolled %d\n\n", getName(players[playerIndex].color), diceNumber);

      // game loop if all pieces are still in base
      // if (noOfPiecesInBase == PLAYER_NO && canMoveToBoard(diceNumber))
      // {
      //   moveFromBase(&players[playerIndex], &players[playerIndex].pieces[0], standardCells[players[playerIndex].startIndex]);
      //   // implement re throw functionalities (later)

      //   // while (canMoveToBoard(diceNumber))
      //   // {
      //   //  diceNumber = rollDice();
      //   // }
      //   // printf("%s is moving %s\n", getName(players[playerIndex].color), players[playerIndex].pieces[0].clockWise ? "clockwise" : "anti-clockwise");
      // }
      // else
      // {
      //   switch (players[playerIndex].color)
      //   {
      //     case RED:
      //       redMoveParse(players, playerIndex, diceNumber, standardCells);
      //       break;
      //   }
      // }

      int minConsecutive = 0;
      int captureCount = getCaptureCountOfPlayer(&players[playerIndex]);

      do
      {
        switch (players[playerIndex].color)
        {
          case RED:
            redMoveParse(players, playerIndex, diceNumber, standardCells);
            break;
        }

        minConsecutive++;

        int newCaptureCount = getCaptureCountOfPlayer(&players[playerIndex]);

        if (diceNumber != MAX_DICE_VALUE && newCaptureCount <= captureCount)
        {
          break;
        }
        else
        {
          diceNumber = rollDice();
          printf("%s player rolled %d\n\n", getName(players[playerIndex].color), diceNumber);
        }

      } while (minConsecutive < 3);
    } 

    displayPlayerStatusAfterRound(players, game);
    displayMysteryCellStatusAfterRound(game->mysteryCellNo, game->mysteryRounds);
    
    printf("\n");
    // test incremental counter
    test++;

    //test loop stop
    if (test >= 150)
    {
      break;
    }
  }
}