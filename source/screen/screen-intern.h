/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-22
 */

#ifndef SCREEN_INTERN_H
#define SCREEN_INTERN_H

Point parse_mouse_point(SDL_Event, Screen, Board);

bool parse_board_input(Board*, SDL_Event, Screen);

bool make_screen_window(SDL_Window**, char[], int, int);

bool make_surface_texture(SDL_Texture**, SDL_Renderer*, SDL_Surface*);

bool setup_screen_struct(Screen*, char[], int, int);

void free_screen_struct(Screen);

bool render_field_square(GUI, Board, Point, Square);

bool extract_square_image(SDL_Surface**, Square, Images);

bool screen_field_point(SDL_Rect*, Screen, Board, Point);

bool extract_symbol_image(SDL_Surface**, Square, Images);

bool render_screen_text(Screen, char[], TTF_Font*, SDL_Color, SDL_Rect);

bool extract_file_font(TTF_Font**, char[], int);

bool render_board_options(GUI);

bool extract_file_image(SDL_Surface**, char[]);

bool render_file_image(Screen, char[], SDL_Rect);

bool render_screen_image(Screen, SDL_Surface*, SDL_Rect);

void free_sounds_struct(Sounds);

void free_images_struct(Images);

bool render_result_message(GUI, Result);

bool render_field_ground(GUI);

bool render_board_ground(GUI);

bool board_options_position(SDL_Rect*, int, Screen);

bool result_message_position(SDL_Rect*, char[], GUI);

bool text_true_dimensions(int*, int*, char[], TTF_Font*);

void free_fonts_struct(Fonts);

bool setup_fonts_struct(Fonts*);

#endif // SCREEN_INTERN_H
