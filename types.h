#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#define PLAYER_NO 4
#define PIECE_NO 4
#define MAX_STANDARD_CELL 52
#define MAX_HOME_STRAIGHT 20
#define MAX_COLORS 4
#define APPROACH_DIFFERENCE 2
#define MYSTERY_LOCATIONS 6

enum Color {
  YELLOW,
  BLUE,
  RED,
  GREEN
};

enum Locations {
  BASE = -1,
  HOME = -2,
  BHAWANA = 9,
  KOTUWA = 27,
  PITA_KOTUWA = 46,
};

enum StartLocations {
  YELLOW_START = 0,
  BLUE_START = 13,
  RED_START = 26,
  GREEN_START = 39,
};

enum ApproachLocations {
  YELLOW_APPROACH = MAX_STANDARD_CELL - APPROACH_DIFFERENCE,
  BLUE_APPROACH = BLUE_START - APPROACH_DIFFERENCE,
  RED_APPROACH = RED_START - APPROACH_DIFFERENCE,
  GREEN_APPROACH = GREEN_START - APPROACH_DIFFERENCE
};

struct MysteryEffects
{
  bool effectActive;
  bool pieceActive;
  int effectActiveRounds;
  int diceMultiplier;
  int diceDivider;
};

struct Piece
{
  int cellNo;
  int captured;
  char name[3];
  bool clockWise;
  bool block;
  int noOfApproachPasses;
  struct MysteryEffects effect;
};

struct Game
{
  int rounds;
  int mysteryCellNo;
  int mysteryRounds;
  int order[PLAYER_NO];
  int win[PLAYER_NO];
  int prevMysteryCells[MAX_STANDARD_CELL];
};

struct Player
{
  int startIndex;
  struct Piece pieces[4];
  enum Color color;
};

#endif // !TYPES_H