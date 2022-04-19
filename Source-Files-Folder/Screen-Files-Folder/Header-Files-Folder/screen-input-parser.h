
#ifndef SCREEN_INPUT_PARSER_H
#define SCREEN_INPUT_PARSER_H

Input input_screen_point(Point*, Field, Board, Stats*, GUI*);

Point parse_mouse_point(Event, Screen, Board);

bool input_screen_board(Board*, GUI*);

bool parse_board_input(Board*, Event, Screen);

#endif
