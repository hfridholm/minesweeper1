
#ifndef SCREEN_SUPPORT_FILE_H
#define SCREEN_SUPPORT_FILE_H

bool make_screen_window(Window**, char[], uint16_t, uint16_t);

bool make_surface_texture(Texture**, Render*, Surface*);

bool setup_screen_struct(Screen*, char[], uint16_t, uint16_t);

void free_screen_struct(Screen);

bool render_field_square(Screen, Board, Point, Square, Images);

bool render_mine_field(Screen, Field, Board, Images);

bool extract_square_image(Image**, Square, Images);

bool screen_field_point(Rect*, Screen, Board, Point);

bool extract_symbol_image(Image**, Square, Images);

bool render_screen_text(Screen, char[], Color, int, int, float);

bool extract_file_font(Font**, char[], int);

bool render_board_options(Screen);

bool extract_file_image(Surface**, char[]);

bool render_file_image(Screen, char[], Rect);

bool render_screen_image(Screen, Surface*, Rect);

void free_sounds_struct(Sounds);

void free_images_struct(Images);

#endif
