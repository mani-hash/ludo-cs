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
  if (diceNumber == MAX_DICE_VALUE)
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

int getEnemyCountOfCell(struct Piece *cell[PLAYER_NO], enum Color playerColor)
{
  int count = 0;
  for (int cellIndex = 0; cellIndex < PLAYER_NO; cellIndex++)
  {
    if (
      cell[cellIndex] != NULL 
      &&
      getPieceColor(cell[cellIndex]->name[0]) != playerColor
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

int getMysteryEffectNumber(int location)
{
  switch (location)
  {
  case BHAWANA:
    return 1;
  case KOTUWA:
    return 2;
  case PITA_KOTUWA:
    return 3;
  case BASE:
    return 4;
  case RED_START:
  case BLUE_START:
  case GREEN_START:
  case YELLOW_START:
    return 5;
  case RED_APPROACH:
  case BLUE_APPROACH:
  case GREEN_APPROACH:
  case YELLOW_APPROACH:
    return 6;
  }
}

int getMysteryLocation(int mysteryEffect, struct Piece *piece)
{
  int mysteryLocation = EMPTY;
  switch (mysteryEffect)
  {
    case 1:
      mysteryLocation = BHAWANA;
      break;
    case 2:
      mysteryLocation = KOTUWA;
      break;
    case 3:
      mysteryLocation = PITA_KOTUWA;
      break;
    case 4:
      mysteryLocation = BASE;
      break;
    case 5:
      mysteryLocation = getStartIndex(getPieceColor(piece->name[0]));
      break;
    case 6:
      mysteryLocation = getApproachIndex(getPieceColor(piece->name[0]));
      break;
  }

  return mysteryLocation;
}

char *getMysteryLocationName(int mysteryEffect)
{
  switch (mysteryEffect)
  {
    case 1:
      return "bhawana";
    case 2:
      return "kotuwa";
    case 3:
      return "pita kotuwa";
    case 4:
      return "base";
    case 5:
      return "X";
    case 6:
      return "approach";
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

int getDistanceFromHome(struct Piece *piece)
{
  enum Color color = getPieceColor(piece->name[0]);
  int startIndex = getStartIndex(color);
  int approachIndex = getApproachIndex(color);

  int distanceFromHome = 0;

  if (piece->cellNo >= MAX_STANDARD_CELL)
  {
    distanceFromHome = (MAX_STANDARD_CELL + HOME_STRAIGHT_DISTANCE) - piece->cellNo;
    return distanceFromHome;
  }

  if (approachIndex >= piece->cellNo)
  {
    distanceFromHome = (approachIndex - piece->cellNo);
  }
  else
  {
    distanceFromHome = (piece->cellNo - approachIndex);

    if (color == YELLOW && piece->cellNo > RED_APPROACH)
    {
      distanceFromHome = MAX_STANDARD_CELL - distanceFromHome;
    }
  }

  distanceFromHome += HOME_STRAIGHT_DISTANCE;
  return distanceFromHome;
}

int getEnemyDistanceFromHome(struct Piece *cell[PIECE_NO])
{
  int enemyDistanceFromHome = 0;

  for (int cellIndex = 0; cellIndex < PIECE_NO; cellIndex++)
  {
    if (cell[cellIndex] != NULL)
    {
      enemyDistanceFromHome = getDistanceFromHome(cell[cellIndex]);
      break;
    }
  }

  return enemyDistanceFromHome;
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

void formBlock(struct Piece *cell[PIECE_NO])
{
  int maxDistanceFromHome = 0;
  bool selectedClockWise = true;

  if (cell == NULL)
  {
    printf("Oh boy this array is null\n");
  }
  for (int cellIndex = 0; cellIndex < PIECE_NO; cellIndex++)
  {
    if (cell[cellIndex] != NULL)
    {
      int distanceFromHome = getDistanceFromHome(cell[cellIndex]);
      if (distanceFromHome > maxDistanceFromHome)
      {
        maxDistanceFromHome = distanceFromHome;
        selectedClockWise = cell[cellIndex]->clockWise;
      }
      printf("%s is name of piece", cell[cellIndex]->name);
      cell[cellIndex]->block = true;
    }
  }

  // set direction for block piece
  for (int cellIndex = 0; cellIndex < PIECE_NO; cellIndex++)
  {
    if (cell[cellIndex] != NULL)
    {
      cell[cellIndex]->blockClockWise = selectedClockWise;
    }
  }
}

void destroyBlock(struct Piece *cell[PIECE_NO])
{
  for (int cellIndex = 0; cellIndex < PIECE_NO; cellIndex++)
  {
    if (cell[cellIndex] != NULL)
    {
      cell[cellIndex]->block = false;
    }
  }
}

void moveFromBase(struct Player *player, struct Piece *piece, struct Piece *cell[PLAYER_NO])
{
  enum Color color = getPieceColor(piece->name[0]);
  char *pieceColor = getName(color);
  int enemyCount = getEnemyCountOfCell(cell, color);
  int playerCount = getPlayerCountOfCell(cell, color);

  if (!isBlocked(1, enemyCount))
  {
    for (int cellIndex = 0; cellIndex < PLAYER_NO; cellIndex++)
    {
      if (cell[cellIndex] != NULL && cell[cellIndex]->name[0] != piece->name[0])
      {
        cell[cellIndex]->cellNo = BASE;
        cell[cellIndex] = NULL;
        piece->captured += 1;
        break;
      }
    }

    for (int cellIndex = 0; cellIndex < PIECE_NO; cellIndex++)
    {
      if (cell[cellIndex] == NULL)
      {
        cell[cellIndex] = piece;
        break;
      }
    }
    piece->cellNo = player->startIndex;
    piece->clockWise = getDirectionFromToss();

    if (playerCount != 0)
    {
      formBlock(cell); 
    }

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

void applyMysteryEffect(int mysteryEffect, int mysteryLocation, struct Piece *piece, char *playerName, char *pieceName)
{
  switch (mysteryEffect)
  {
    case 1: // bhawana
      int energy = rand() % 2;
      piece->effect.effectActive = true;
      piece->effect.effectActiveRounds = 4;

      if (energy)
      {
        printf("%s piece %s feels energized and movement speed doubles", playerName, pieceName);
        piece->effect.diceMultiplier = 2;  
      }
      else
      {
        printf("%s piece %s feels sick and movement speed halves", playerName, pieceName);
        piece->effect.diceDivider = 2;
      }

      break;
    case 2: // kotuwa
      piece->effect.effectActive = true;
      piece->effect.pieceActive = false;
      piece->effect.effectActiveRounds = 4;
      printf("%s piece %s attends meeting and cannot move for the next four rounds", playerName, pieceName);
      break;
    case 3: // pita kotuwa
      if (piece->clockWise)
      {
        printf("%s piece %s which was moving clockwise, has changed to moving counter-clockwise", playerName, pieceName);
        piece->clockWise = false;
      }
      else
      {
        printf("%s piece %s is moving in counter clockwise direction direction. Teleporting to Kotuwa from pitakotuwa", playerName, pieceName);

        piece->effect.effectActive = true;
        piece->effect.pieceActive = false;
        piece->effect.effectActiveRounds = 4; 
      }
      break;
  }

  piece->cellNo = mysteryLocation;
}

void applyTeleportation(struct Piece **pieces, int mysteryEffect, int count, struct Piece *cells[][PLAYER_NO])
{
  int mysteryLocation = getMysteryLocation(mysteryEffect, pieces[0]);
  char *mysteryLocationName = getMysteryLocationName(mysteryEffect);
  enum Color color = getPieceColor(pieces[0]->name[0]);
  char *playerName = getName(color);

  if (mysteryLocation == BASE)
  {
    for (int pieceIndex = 0; pieceIndex < count; pieceIndex++)
    {
      displayTeleportationMessage(playerName, count, pieces[pieceIndex], mysteryLocationName);
      pieces[pieceIndex]->cellNo = BASE;
    }
    return;
  }

  int enemyCount = getEnemyCountOfCell(cells[mysteryLocation], color);

  if (isBlocked(count, enemyCount))
  {
    printf("There is a block in L%d preventing %s from teleporting therefore aborting mystery cell teleportation\n",
      mysteryLocation,
      playerName
    );
    return;
  }

  bool captured = false;
  for (int cellIndex = 0; cellIndex < PLAYER_NO; cellIndex++)
  {
    // turn this if condition check into a function
    if (cells[mysteryLocation][cellIndex] != NULL && cells[mysteryLocation][cellIndex]->name[0] != pieces[0]->name[0])
    {
      captured = true;
    }
  }

  for (int pieceIndex = 0; pieceIndex < count; pieceIndex++)
  {
    if (captured)
    {
      if (pieces[pieceIndex]->block)
      {
        captureByBlock(pieces, count, cells, mysteryLocation, playerName);
      }
      else
      {
        captureByPiece(pieces[0], cells, mysteryLocation, playerName);
      }
    }

    for (int cellIndex = 0; cellIndex < PLAYER_NO; cellIndex++)
    {
      if (cells[mysteryLocation][cellIndex] != NULL)
      {
        continue;
      }
      else
      {
        cells[mysteryLocation][cellIndex] = pieces[pieceIndex];
        displayTeleportationMessage(playerName, count, pieces[pieceIndex], mysteryLocationName);
        applyMysteryEffect(mysteryEffect, mysteryLocation, pieces[pieceIndex], playerName, pieces[pieceIndex]->name);

        if (mysteryEffect == getMysteryEffectNumber(PITA_KOTUWA) && !pieces[pieceIndex]->clockWise)
        {
          // int newMysteryLocation = getMysteryLocation(getMysteryEffectNumber(KOTUWA), pieces[pieceIndex]);
          int newMysteryEffect = getMysteryEffectNumber(KOTUWA);
          struct Piece **newPieces = {&pieces[pieceIndex]};

          applyTeleportation(newPieces, newMysteryEffect, 1, cells);
        }
        break;
      }
    }
  }
}

int getDiceValueAfterMysteryEffect(int diceNumber, struct Player *player, int pieceIndex)
{
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

  return diceNumber;
}

void playRound()
{

}

void captureByPiece(struct Piece *piece, struct Piece *cells[][PIECE_NO], int finalCellNo, char *playerName)
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

void captureByBlock
(
  struct Piece **blockPieces,
  int playerCount,
  struct Piece *cells[][PIECE_NO],
  int finalCellNo,
  char *playerName
)
{
  for (int cellIndex = 0, blockIndex = 0; cellIndex < PIECE_NO; cellIndex++)
  {
    if (cells[finalCellNo][cellIndex] != NULL)
    {
      enum Color enemyColor = getPieceColor(cells[finalCellNo][cellIndex]->name[0]);
      char *enemyName = getName(enemyColor);

      printf("%s piece %s is captured by block of %s and is returned to the base\n",
        enemyName,
        cells[finalCellNo][cellIndex]->name,
        playerName
      );
          
      cells[finalCellNo][cellIndex]->cellNo = BASE;
      // add reset piece functionality later
      cells[finalCellNo][cellIndex] = NULL;
    }
  }

  for (int blockIndex = 0; blockIndex < playerCount; blockIndex++)
  {
    blockPieces[blockIndex]->captured++;
  }
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

  bool formBlockStatus = false; 
  int directonConstant = piece->clockWise ? 1 : -1;
  int finalCellNo = getCorrectCellCount(piece->cellNo + (diceNumber * directonConstant));

  displayMovablePieceStatus(movableCellCount, diceNumber, playerName, piece, &finalCellNo, cells[finalCellNo]);

  cells[piece->cellNo][pieceIndex] = NULL;
  piece->cellNo = finalCellNo;
  piece->block = false;
    
  if (!isCellEmpty(cells[finalCellNo]))
  {
    if (getEnemyCountOfCell(cells[finalCellNo], color) != 0)
    {
      captureByPiece(piece, cells, finalCellNo, playerName);
    }
    else
    {
      formBlockStatus = true;
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

  if (formBlockStatus)
  {
    formBlock(cells[finalCellNo]);
    printf("%s piece has formed a block on L%d",
      playerName,
      finalCellNo
    );
  }
}

void moveBlock(struct Piece *piece, int diceNumber, struct Piece *cells[][PIECE_NO])
{
  enum Color color = getPieceColor(piece->name[0]);
  char *playerName = getName(color);
  int playerCount = getPlayerCountOfCell(cells[piece->cellNo], color);
  struct Piece *blockPieces[playerCount];

  // create a block piece array to keep track of block
  for (int cellIndex = 0, blockIndex = 0; cellIndex < playerCount; cellIndex++)
  {
    if (cells[piece->cellNo][cellIndex] != NULL)
    {
      blockPieces[blockIndex] = cells[piece->cellNo][cellIndex];
    }
  }
  
  int movableCellCount = 
    getMovableCellCount
    (
      piece->cellNo,
      diceNumber,
      piece->blockClockWise,
      playerCount,
      cells,
      color
    );

  bool formBlockStatus = false;
  int directonConstant = piece->blockClockWise ? 1 : -1;
  int finalCellNo = getCorrectCellCount(piece->cellNo + (diceNumber * directonConstant));

  displayMovableBlockStatus(movableCellCount, diceNumber, playerName, piece, &finalCellNo, cells[finalCellNo]);

  // exit if movable cell count is 0
  if (movableCellCount == 0)
  {
    return;
  }

  if (!isCellEmpty(cells[finalCellNo]))
  {
    if (getEnemyCountOfCell(cells[finalCellNo], color) != 0)
    {
      captureByBlock(blockPieces, playerCount, cells, finalCellNo, playerName);
    }
    else
    {
      formBlockStatus = true; 
    }
  }

  for (int cellIndex = 0, blockIndex = 0; cellIndex < PIECE_NO; cellIndex++)
  {
    if (cells[finalCellNo][cellIndex] == NULL)
    {
      cells[finalCellNo][blockIndex] = blockPieces[blockIndex];

      blockIndex++;
    }
  }

  if (formBlockStatus)
  {
    formBlock(cells[finalCellNo]);
    printf("Block of %s has formed another block",
      playerName
    );
  }
}

void handlePieceLandOnMysteryCell(struct Game *game, struct Player *player, struct Piece *cells[][PIECE_NO])
{
  int count = 0;
  for (int pieceIndex = 0; pieceIndex < PIECE_NO; pieceIndex++)
  {
    if (game->mysteryCellNo == player->pieces[pieceIndex].cellNo)
    {
      count++;
    }
  }

  if (count == 0)
  {
    return;
  }

  struct Piece *pieces[count];

  for (int pieceIndex = 0, pieceCounter = 0; pieceIndex < PIECE_NO; pieceIndex++)
  {
    if (game->mysteryCellNo == player->pieces[pieceIndex].cellNo)
    {
      pieces[pieceCounter] = &player->pieces[pieceIndex];
      pieceCounter++;
    }
  }

  if (pieces != NULL)
  {
    int mysteryEffect = getMysteryEffect();
    applyTeleportation(pieces, mysteryEffect, count, cells);
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

  // variables to track piece importance
  int pieceImportance[] = { 5, 5, 5, 5 };
  int canAttackCount = 0;

  for (int pieceIndex = 0; pieceIndex < PIECE_NO; pieceIndex++)
  {

    // check mystery effects
    diceNumber = getDiceValueAfterMysteryEffect(diceNumber, player, pieceIndex);

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

    validateRedPieceImportance(piecePriorities, pieceImportance, pieceIndex, &canAttackCount, player->pieces[pieceIndex].block);
  }

  int selectedPiece = getIndexOfSelectedRedPiece(player->pieces, cells, pieceImportance, canAttackCount, diceNumber);

  finalizeRedMovement(player, selectedPiece, diceNumber, cells, piecePriorities[selectedPiece].canExitBlock);
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

  int movableCellCount = 
    getMovableCellCount
    (
      cellNo,
      diceNumber, 
      player->pieces[pieceIndex].clockWise,
      playerCount,
      cells,
      player->color
    );

  if (movableCellCount < diceNumber)
  {
    piecePriorities[pieceIndex].canAttack = false;
    piecePriorities[pieceIndex].canFullMove = false;

    if (movableCellCount != 0)
    {
      piecePriorities[pieceIndex].canPartialMove = true;

    }
  }
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
  diceNumber /= playerCount;

  int movableCellCount =
    getMovableCellCount(
      cellNo,
      diceNumber,
      player->pieces[pieceIndex].clockWise,
      playerCount,
      cells,
      player->color
    );

  if (movableCellCount < diceNumber)
  {
    piecePriorities[pieceIndex].canFullMove = false;
    piecePriorities[pieceIndex].canAttack = false;

    if (movableCellCount != 0)
    {
      piecePriorities[pieceIndex].canPartialMove = true;
    }
  }
}

void validateRedPieceImportance(
  struct RedPriority *piecePriorities,
  int *pieceImportance,
  int pieceIndex,
  int *canAttackCount,
  bool blockade
)
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
      return;
    }

    if (!piecePriorities[pieceIndex].canFullMove)
    {
      pieceImportance[pieceIndex] -= 1;
    }
      
    if (!piecePriorities[pieceIndex].canPartialMove)
    {
      pieceImportance[pieceIndex] -= 1;
    }

    if (blockade && !piecePriorities[pieceIndex].canExitBlock)
    {
      pieceImportance[pieceIndex] -= 1;
    }
  }

  if (pieceImportance[pieceIndex] == 5)
  {
    *canAttackCount++;
  }
}

int getIndexOfSelectedRedPiece
(
  struct Piece *pieces,
  struct Piece *cells[][PIECE_NO],
  int *pieceImportance,
  int canAttackCount,
  int diceNumber
)
{
  int selectedPiece = EMPTY;
  int maxPriority = EMPTY;
  int prevEnemyDistanceFromHome = MAX_STANDARD_CELL;

  for (int pieceIndex = 0; pieceIndex < PIECE_NO; pieceIndex++)
  {
    if (pieceImportance[pieceIndex] > maxPriority)
    {
      maxPriority = pieceImportance[pieceIndex];
      selectedPiece = pieceIndex;
    }

    // Selects the opponent player close to their home if there are multiple
    // attack options
    if (pieceImportance[pieceIndex] == 5 && canAttackCount != 0)
    {
      int destinationIndex = pieces[pieceIndex].cellNo + diceNumber;
      int enemyDistanceFromHome = getEnemyDistanceFromHome(cells[destinationIndex]);

      if (enemyDistanceFromHome < prevEnemyDistanceFromHome)
      {
        selectedPiece = pieceIndex;
      }
    }
  }

  return selectedPiece;
}

void finalizeRedMovement
(
  struct Player *player,
  int selectedPiece, 
  int diceNumber, 
  struct Piece *cells[][PIECE_NO], 
  bool canExitBlock
)
{
  if (player->pieces[selectedPiece].cellNo == BASE && diceNumber == MAX_DICE_VALUE)
  {
    moveFromBase(player, &player->pieces[selectedPiece], cells[getStartIndex(player->color)]);
  }
  else if (player->pieces[selectedPiece].cellNo != BASE && player->pieces[selectedPiece].cellNo != HOME)
  {
    if (player->pieces[selectedPiece].block && !canExitBlock)
    {
      moveBlock(&player->pieces[selectedPiece], diceNumber, cells);
    }
    else
    {
      move(&player->pieces[selectedPiece], selectedPiece, diceNumber, cells);
    }
  }
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
  if (mysteryCellNo == EMPTY)
  {
    printf("The required conditions for generating mystery cells have not been met\n");
  }
  else
  {
    printf("The mystery cell is at L%d and will be at that location for the next %d rounds\n",
      mysteryCellNo,
      mysteryRounds
    );
  }
}

void displayTeleportationMessage(char* playerName, int count, struct Piece *pieces, char *location)
{
  printf("%s piece ", playerName);
  for (int pieceIndex = 0; pieceIndex < count; pieceIndex++)
  {
    printf("%s ", pieces[pieceIndex].name);
  }
  printf("teleported to %s\n", location);
}

void displayMovablePieceStatus
(
  int movableCellCount,
  int diceNumber,
  char *playerName,
  struct Piece *piece,
  int *finalCellNo,
  struct Piece *cell[PIECE_NO]
)
{
  if (movableCellCount == 0)
  {
    printf("%s piece %s is blocked from L%d to L%d by %s piece\n",
      playerName,
      piece->name,
      piece->cellNo,
      *finalCellNo,
      getName(getPlayerColorInCell(cell))
    );

    printf("%s does not have other pieces to move instead of blocked piece. ", playerName);
    printf("Ignoring the throw and moving to the next player\n");

    return;
  }
  else if (movableCellCount < diceNumber)
  {
    int directionConstant = piece->clockWise ? 1 : -1;
    *finalCellNo = getCorrectCellCount(piece->cellNo + (movableCellCount * directionConstant));

    printf("%s does not have other pieces to move instead of blocked piece. Moved the piece %s to square L%d which is a cell before the block\n",
      playerName,
      piece->name,
      *finalCellNo
    );
  }
  else
  {
    printf("%s moves piece %s from location L%d to L%d by %d units in %s direction\n",
      playerName,
      piece->name,
      piece->cellNo,
      *finalCellNo,
      movableCellCount,
      piece->clockWise ? "clock-wise" : "counter clock-wise"
    );
  }
}

void displayMovableBlockStatus(
  int movableCellCount,
  int diceNumber,
  char *playerName,
  struct Piece *piece,
  int *finalCellNo,
  struct Piece *cell[PIECE_NO]
)
{
  if (movableCellCount == 0)
  {
    printf("Block of %s has been blocked by %s block from moving from L%d to L%d",
      playerName,
      getName(getPlayerColorInCell(cell)),
      piece->cellNo,
      *finalCellNo
    );
    printf("%s does not have other pieces to move instead of blocked piece. ", playerName);
    printf("Ignoring the throw and moving to the next player\n");

    return;
  }
  else if (movableCellCount < diceNumber)
  {
    int directionConstant = piece->blockClockWise ? 1: -1;
    *finalCellNo = getCorrectCellCount(piece->cellNo + (movableCellCount * directionConstant));

    printf("%s does not have other pieces to move instead of blocked piece. Moved the block pieces to square L%d which is a cell before the block\n",
      playerName,
      *finalCellNo
    );
  }
  else
  {
    printf("Block of %s moves from location L%d to L%d by %d units in %s direction",
      playerName,
      piece->cellNo,
      *finalCellNo,
      movableCellCount,
      piece->blockClockWise ? "clock-wise" : "counter clock-wise"
    );
  }
}

/* Functions for game loop
 */

void initialGameLoop(struct Player *players, struct Game *game)
{
  int max = EMPTY;
  int maxPlayerIndex = EMPTY;

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
  if (game->roundsTillMysteryCell < 2)
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
      printf("A mystery cell has spawned in location L%d and will be at this location for the next %d rounds\n",
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

        // handle piece landing on mystery cell
        handlePieceLandOnMysteryCell(game, &players[playerIndex], standardCells);

        int newCaptureCount = getCaptureCountOfPlayer(&players[playerIndex]);

        if (diceNumber != MAX_DICE_VALUE && newCaptureCount <= captureCount)
        {
          break;
        }
        else
        {
          diceNumber = rollDice();
          printf("%s player rolled %d\n\n", getName(players[playerIndex].color), diceNumber);

          if (newCaptureCount > captureCount)
          {
            minConsecutive = 0;
          }
        }

      } while (minConsecutive < 3);
    } 

    displayPlayerStatusAfterRound(players, game);
    displayMysteryCellStatusAfterRound(game->mysteryCellNo, game->mysteryRounds);
    
    printf("\n");
    // test incremental counter
    test++;

    //test loop stop
    if (test >= 10)
    {
      break;
    }
  }
}