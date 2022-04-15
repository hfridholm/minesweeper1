
#ifndef SCREEN_INPUT_PARSER_H
#define SCREEN_INPUT_PARSER_H

Input input_screen_point(Point*, Screen, Field, const Board);

Point parse_mouse_point(const Event, Screen, const Board);

#endif
