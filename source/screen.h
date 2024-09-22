/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-22
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string.h>

#include "minesweeper.h"

#include "screen/screen-struct.h"

bool game_action_handler(Field, Board, Stats*, GUI*);

bool setup_gui_struct(GUI*, char[], int, int);

bool input_screen_board(Board*, GUI*);

void free_gui_struct(GUI);

bool game_result_handler(Field, Board, Stats, Result, GUI*);

bool render_result_screen(GUI, Field, Board, Stats, Result);

bool render_mine_field(GUI, Field, Board, Stats);

Input input_screen_point(Point*, Field, Board, Stats*, GUI*);

bool point_input_handler(Field, Board, Input, Point, GUI);

#endif // SCREEN_H

