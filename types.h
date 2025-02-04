#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>
#define PLAYER_NO 4
#define PIECE_NO 4
#define EMPTY -999
#define MAX_DICE_VALUE 6
#define MAX_STANDARD_CELL 52
#define HOME_STRAIGHT_DISTANCE 5
#define APPROACH_DIFFERENCE 2
#define MYSTERY_LOCATIONS 6
#define MAX_PRIORITY 10

enum Color {
  YELLOW,
  BLUE,
  RED,
  GREEN
};

enum Locations {
  BASE = -1,
  HOME = MAX_STANDARD_CELL + HOME_STRAIGHT_DISTANCE,
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
} __attribute__((aligned(4)));

struct Piece
{
  int cellNo;
  int captured;
  int noOfApproachPasses;
  bool clockWise;
  bool blockClockWise;
  uint8_t reserved : 6; 
  char name[3];
  struct MysteryEffects effect;
} __attribute__((aligned(4)));

struct Game
{
  int rounds;
  int mysteryCellNo;
  int mysteryRounds;
  int roundsTillMysteryCell;
  int winIndex;
  int order[PLAYER_NO];
  int winners[PLAYER_NO];
  int prevMysteryCell;
} __attribute__((aligned(4)));

struct Player
{
  int startIndex;
  struct Piece pieces[4];
  enum Color color;
} __attribute__((aligned(4)));

struct RedPriority
{
  bool canMoveFromBase;
  bool canFullMove;
  bool canPartialMove;
  bool canAttack;
  bool canFormBlock;
  bool canExitBlock;
} __attribute__((aligned(4)));

struct GreenPriority
{
  bool canMoveFromBase;
  bool isBlockMovable;
  bool canFullMove;
  bool canPartialMove;
  bool canFormBlock;
} __attribute__((aligned(4)));

struct YellowPriority
{
  bool canMoveFromBase;
  bool canFullMove;
  bool canPartialMove;
  bool canAttack;
  bool canExitBlock;
} __attribute__((aligned(4)));

struct BluePriority
{
  bool canFullMove;
  bool canPartialMove;
  bool preferToMove;
  bool canExitBlock;
} __attribute__((aligned(4)));

union PiecePriority
{
  struct RedPriority *redPriority;
  struct GreenPriority *greenPriority;
  struct YellowPriority *yellowPriority;
  struct BluePriority *bluePriority;
} __attribute__((aligned(8)));

#endif // !TYPES_H