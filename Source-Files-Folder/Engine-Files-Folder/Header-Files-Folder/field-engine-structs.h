
#ifndef ENGINE_STRUCT_DEFINES_H
#define ENGINE_STRUCT_DEFINES_H

typedef struct Square
{
  bool mine;
  bool visable;
  bool flagged;
  int adjacent;
} Square;

typedef struct Point
{
  int height;
  int width;
} Point;

typedef struct Board
{
  int height;
  int width;
  int mines;
} Board;

#define STUPID_BOARD (Board) {9, 9, 10};
#define NORMAL_BOARD (Board) {16, 16, 40};
#define EXPERT_BOARD (Board) {16, 30, 99};

typedef Square** Field;

#endif
