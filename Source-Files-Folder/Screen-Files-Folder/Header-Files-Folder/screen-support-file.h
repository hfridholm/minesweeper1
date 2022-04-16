
#ifndef SCREEN_SUPPORT_FILE_H
#define SCREEN_SUPPORT_FILE_H

bool make_screen_window(Window**, char[], uint16_t, uint16_t);

bool make_surface_texture(Texture**, Render*, Surface*);

bool setup_screen_struct(Screen*, char[], uint16_t, uint16_t);

void free_screen_struct(Screen);

bool color_point_square(Screen, Board, Point, Color);

bool render_field_square(Screen, Board, Point, Square);

bool render_field_symbol(Screen, Board, Point, Square);

bool render_mine_field(Screen, Field, Board);

bool render_surface_texture(Render*, Surface*, Rect);

bool extract_square_file(char*, Square);

bool extract_square_image(Surface**, Square);

bool screen_field_point(Rect*, Screen, Board, Point);

bool extract_symbol_image(Surface**, Square);

bool extract_symbol_file(char*, Square);

bool render_screen_text(Screen, char[], Color, int, int, float);

bool extract_file_font(Font**, char[], int);

bool render_board_options(Screen);

bool extract_file_image(Surface**, char[]);

bool render_file_image(Screen, char[], Rect);

bool render_screen_image(Screen, Surface*, Rect);

void free_sounds_struct(Sounds);

#endif
