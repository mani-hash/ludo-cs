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
