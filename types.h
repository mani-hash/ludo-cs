#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#define PLAYER_NO 4
#define PIECE_NO 4
#define MAX_DICE_VALUE 6
#define MAX_STANDARD_CELL 52
#define HOME_STRAIGHT_DISTANCE 5
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
  YELLOW_START = 2,
  BLUE_START = 15,
  RED_START = 28,
  GREEN_START = 41,
};

enum ApproachLocations {
  YELLOW_APPROACH = YELLOW_START - APPROACH_DIFFERENCE,
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
  bool blockClockWise;
  int noOfApproachPasses;
  struct MysteryEffects effect;
};

struct Game
{
  int rounds;
  int mysteryCellNo;
  int mysteryRounds;
  int roundsTillMysteryCell;
  int order[PLAYER_NO];
  int win[PLAYER_NO];
  int prevMysteryCell;
};

struct Player
{
  int startIndex;
  struct Piece pieces[4];
  enum Color color;
};

struct RedPriority
{
  bool canMoveFromBase;
  bool canFullMove;
  bool canPartialMove;
  bool canAttack;
  bool canFormBlock;
  bool canExitBlock;
};

struct GreenPriority
{

};

struct YellowPriority
{

};

struct BluePriority
{

};

#endif // !TYPES_H