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

#endif // !TYPES_H