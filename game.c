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

void move(struct Piece *piece, int diceNumber, struct Piece *cells[][PIECE_NO])
{

}

/* Behavior functions
 */

void redMoveParse(struct Player *player, int redPlayerIndex, int diceNumber, struct Piece *cells[][PIECE_NO])
{
  int selectedPieceIndex = -1;

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
        continue;
      }

      diceNumber *= effect->diceMultiplier;
      diceNumber /= effect->diceDivider;
    }

    if (!initialRedMovementCheck(player, piecePriorities, cells, pieceIndex, diceNumber))
    {
      continue;
    }

    validateRedMovement(player, piecePriorities, cells, pieceIndex, diceNumber);

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
  int maxCellNo = cellNo + diceNumber;
  int playerCount = 1;
  int enemyCountOfCell = getEnemyCountOfCell(cells[maxCellNo], player->color);
  bool isBlockade = player->pieces[pieceIndex].block;

  // if piece is already at home, no movement can be applied!
  if (cellNo == HOME || diceNumber == 0)
  {
    return false;
  }

  if (cellNo == BASE && !isBlocked(1, getStartIndex(player->color)) && canMoveToBoard(diceNumber))
  {
    piecePriorities[pieceIndex].canMoveFromBase = true;
    return false;
  }

  if (cellNo >= MAX_STANDARD_CELL)
  {
    // handle home straight case here
    piecePriorities[pieceIndex].canFullMove = true;
    return false;
    // maxCellNo -= (maxCellNo - MAX_STANDARD_CELL);

  }
  // int approachCellNo = getApproachIndex(player->color);
  // if (cellNo < approachCellNo)

  if (isBlockade)
  {
    piecePriorities[pieceIndex].canExitBlock = true;
    playerCount = getPlayerCountOfCell(cells[cellNo], player->color);
  }

  if (isCellEmpty(cells[maxCellNo]))
  {
    // when final destination cell is empty
    piecePriorities[pieceIndex].canFullMove = true;
  }
  else if (enemyCountOfCell == 0)
  {
    piecePriorities[pieceIndex].canFormBlock = true;
  }
  else if (!isBlocked(1, enemyCountOfCell))
  {
    piecePriorities[pieceIndex].canAttack = true;
  }
  else if (isBlockade && !isBlocked(playerCount, enemyCountOfCell))
  {
    piecePriorities[pieceIndex].canAttack = true;
    piecePriorities[pieceIndex].canExitBlock = false;
  }

  return true;
}

void validateRedMovement
(
  struct Player *player,
  struct RedPriority *piecePriorities,
  struct Piece *cells[][PIECE_NO],
  int pieceIndex,
  int diceNumber
)
{
  int cellNo = player->pieces[pieceIndex].cellNo;
  int maxCellNo = cellNo + diceNumber;
  int playerCount = 1;
  bool isBlockade = player->pieces[pieceIndex].block;

  if (isBlockade)
  {
    int playerCount = getPlayerCountOfCell(cells[cellNo], player->color);
  }

  for (int cellIndex = cellNo + 1; cellIndex <= maxCellNo; cellIndex++)
  {
    if (!isCellEmpty(cells[cellIndex]))
    {
      int enemyCountOfCell = getEnemyCountOfCell(cells[cellIndex], player->color);
      if (isBlocked(playerCount, enemyCountOfCell))
      {
        piecePriorities[pieceIndex].canAttack = false;
        piecePriorities[pieceIndex].canFullMove = false;

        if (isBlockade)
        {
          int maxBlockCellNo = cellNo + (diceNumber / playerCount);
        }

        if (!cellIndex == cellNo + 1)
        {
          piecePriorities[pieceIndex].canPartialMove = true;
        }

        continue;
      }
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
      if (noOfPiecesInBase == PLAYER_NO && canMoveToBoard(diceNumber))
      {
        moveFromBase(&players[playerIndex], &players[playerIndex].pieces[0], standardCells[players[playerIndex].startIndex]);
        // implement re throw functionalities (later)

        // while (canMoveToBoard(diceNumber))
        // {
        //  diceNumber = rollDice();
        // }
        // printf("%s is moving %s\n", getName(players[playerIndex].color), players[playerIndex].pieces[0].clockWise ? "clockwise" : "anti-clockwise");
      }
    }

    displayPlayerStatusAfterRound(players, game);
    displayMysteryCellStatusAfterRound(game->mysteryCellNo, game->mysteryRounds);
    
    printf("\n");
    // test incremental counter
    test++;

    //test loop stop
    if (test >= 4)
    {
      break;
    }
  }
}