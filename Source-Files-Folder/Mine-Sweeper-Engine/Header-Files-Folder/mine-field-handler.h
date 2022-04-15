
#ifndef MINE_FIELD_HANDLER_H
#define MINE_FIELD_HANDLER_H

Field create_field_matrix(const int, const int);

bool generate_mine_field(Field, const Board);

int adjacent_field_mines(Field, const Board, const Point);

bool activate_field_mines(Field, const Board);

bool mark_adjacent_mines(Field, const Board);

bool point_inside_board(const Point, const Board);

void free_mine_field(Field, const int);

bool unlock_field_square(Field, const Board, const Point);

bool flag_field_square(Field, const Board, const Point);

bool mine_field_cleared(Field, const Board);

bool mine_field_exposed(Field, const Board);

void unlock_field_mines(Field, const Board);

void flag_field_mines(Field, const Board);

bool delete_array_point(Point*, const int, const int);

Point* every_field_point(const int, const int);

#endif
