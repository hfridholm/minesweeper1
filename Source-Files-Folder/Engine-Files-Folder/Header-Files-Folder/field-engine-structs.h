
#ifndef ENGINE_STRUCT_DEFINES_H
#define ENGINE_STRUCT_DEFINES_H

typedef struct Square
{
  bool mine, visable, flagged;
  int adjacent;
} Square;

typedef struct Point
{
  int height, width;
} Point;

typedef struct Board
{
  int height, width, mines;
} Board;

typedef enum Result
{
  RESULT_NONE, RESULT_WIN, RESULT_LOSE
} Result;

extern const Board STUPID_BOARD;
extern const Board NORMAL_BOARD;
extern const Board EXPERT_BOARD;

typedef Square** Field;

#endif
