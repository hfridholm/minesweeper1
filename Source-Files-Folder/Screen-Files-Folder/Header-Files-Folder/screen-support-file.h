
#ifndef SCREEN_SUPPORT_FILE_H
#define SCREEN_SUPPORT_FILE_H

bool make_screen_window(Window**, char[], int, int);

bool make_surface_texture(Texture**, Render*, Surface*);

bool setup_screen_struct(Screen*, char[], int, int);

void free_screen_struct(Screen);

bool render_field_square(GUI, Board, Point, Square);

bool render_mine_field(GUI, Field, Board, Stats);

bool extract_square_image(Image**, Square, Images);

bool screen_field_point(Rect*, Screen, Board, Point);

bool extract_symbol_image(Image**, Square, Images);

bool render_screen_text(Screen, char[], Font*, Color, Rect);

bool extract_file_font(Font**, char[], int);

bool render_board_options(GUI);

bool extract_file_image(Surface**, char[]);

bool render_file_image(Screen, char[], Rect);

bool render_screen_image(Screen, Surface*, Rect);

void free_sounds_struct(Sounds);

void free_images_struct(Images);

void free_gui_struct(GUI);

bool render_result_screen(GUI, Field, Board, Stats, Result);

bool game_result_handler(Field, Board, Stats, Result, GUI*);

bool game_action_handler(Field, Board, Stats*, GUI*);

bool point_input_handler(Field, Board, Input, Point, GUI);

bool render_result_message(GUI, Result);

bool setup_gui_struct(GUI*, char[], int, int);

bool render_field_ground(GUI);

bool render_board_ground(GUI);

bool board_options_position(Rect*, int, Screen);

bool result_message_position(Rect*, char[], GUI);

bool text_true_dimensions(int*, int*, char[], Font*);

void free_fonts_struct(Fonts);

bool setup_fonts_struct(Fonts*);

#endif
