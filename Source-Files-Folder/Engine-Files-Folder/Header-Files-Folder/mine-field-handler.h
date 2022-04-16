
#ifndef MINE_FIELD_HANDLER_H
#define MINE_FIELD_HANDLER_H

Field create_field_matrix(int, int);
bool generate_mine_field(Field, Board);

int adjacent_field_mines(Field, Board, Point);
bool activate_field_mines(Field, Board);
bool mark_adjacent_mines(Field, Board);

bool point_inside_board(Point, Board);

void free_mine_field(Field, Board);

bool unlock_field_square(Field, Board, Point);
bool flag_field_square(Field, Board, Point);

bool mine_field_cleared(Field, Board);
bool mine_field_exposed(Field, Board);

void unlock_field_mines(Field, Board);
void flag_field_mines(Field, Board);

bool delete_array_point(Point*, int, int);
Point* every_field_point(int, int);

#endif
